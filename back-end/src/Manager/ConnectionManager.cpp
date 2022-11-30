#include "ConnectionManager.hpp"
#include <utility>
#include "../http/Request/RequestMaker.hpp"

// The class manage the connections between the servers and the clients.
ConnectionManager::ConnectionManager() : _servers(NULL) , _max_socket(0){}

void	ConnectionManager::start_servers(std::vector<Server*>& servers)
{
	_servers = &servers;
	start_timeout(2, 500000);
	start_all_servers();
}

// initialize the timeout for select
void ConnectionManager::start_timeout(int second, int microsecond)
{
	_timeout.tv_sec = second;
	_timeout.tv_usec = microsecond;
}

void ConnectionManager::start_all_servers()
{
	for (size_t i = 0; i < _servers->size(); i++)
	{
		start_one_server(*((*_servers)[i]));
	}
}

// start listening on the server's sockets and add them to the set and the register
void ConnectionManager::start_one_server(Server& actual_server)
{
	for (size_t i = 0; i < actual_server.size(); i++)
	{
		ServerSocket *socket = &(actual_server[i]);
		socket->listen(100);
		add_socket(&actual_server, socket, _all_sockets);
	}
}

// Redirect the clients to the right server and permit the connection to be non-blocking
void	ConnectionManager::manage_connections()
{
    while (true) 
    {
		_sockets_ready = _all_sockets;
		obtain_sockets_used_by_clients();
		handle_new_connection_or_request();
    }
}

// The function select permit to keep in the set _sockets_ready only the ones used by clients
void  ConnectionManager::obtain_sockets_used_by_clients()
{
	int timeout;
	
	timeout = select(_max_socket + 1, _sockets_ready.get_set(), NULL, NULL, &_timeout); // write fd
	if (timeout < 0)
		exit(1);
	if (timeout == 0)
		std::cerr << "[ERROR] select() timeout\n" << std::endl; // removing clients
}

void ConnectionManager::remove_all_client(){
	std::map<int, Socket *>::iterator it;
	std::map<int, Socket *> copy = _register.getSocket(); //could result in segfault otherwise

	for (it = copy.begin(); it != copy.end(); it++){
		if (!it->second->is_listening_port()) //client socket
		{
			close(it->first);
			_all_sockets.remove_socket(it->first);
			_register.erase_entry(it->first);
		}
	}
	_register.setSocket(copy);
}

//	find all the sockets present in the set
//		- if they are listening sockets that mean that a new client connected, so we need to create a new connection
//		- else they sent an http request
void ConnectionManager::handle_new_connection_or_request()
{
	for (int fd = 0; fd <= _max_socket; fd++)
	{
		if (_sockets_ready.is_in_set(fd))
		{
			if (_register[fd]->is_listening_port() == true)
				new_connection(fd);
			else
				new_request(fd);
		}
	}
}

//	To create a new connection we need to create a ClientSocket, that will communicate with the new client.
//	We also need to add it to the set and the register.
void ConnectionManager::new_connection(int socketFd)
{
	Server			*server;
	ClientSocket	*client;

	server = _register.find_server(socketFd);
	client = new_client(socketFd);
	add_socket(server, client, _all_sockets);
}

// To create a clientSocket, a listening socket need to accept the incomming communication.
ClientSocket *ConnectionManager::new_client(int socketFd)
{
	ServerSocket	*listeningSocket;
	int				clientFd;

	listeningSocket = dynamic_cast<ServerSocket *>(_register[socketFd]);
	clientFd = listeningSocket->accept();
	ClientSocket* client = new ClientSocket(clientFd);
	return (client);
}

void ConnectionManager::new_request(int socketFd)
{
	ClientSocket	*client;
	Server			*server;
	Request			request;


	
	server = _register.find_server(socketFd);
	int	buffer_size = server->get_server_info()->body_limit;
	client = dynamic_cast<ClientSocket *>(_register[socketFd]);
	RequestMaker	requestMaker(server, &request);
	requestMaker.make_request(client->recv(buffer_size));
	server->handle_request(client, &request);
	close_connection(client, server);
}


void ConnectionManager::close_connection(ClientSocket *socket, Server *server)
{
	server->set_status_code(0);
	_all_sockets.remove_socket(socket->sockfd());
	_register.erase_entry(socket->sockfd());
	_max_socket = _register.max();
	socket->close();
	delete socket;
}

void ConnectionManager::add_socket(Server* server, ServerSocket* new_socket, SocketSet& socket_set)
{
	int sockfd = new_socket->sockfd();
	if (sockfd > _max_socket)
		_max_socket = sockfd;
	_register.add_entry(sockfd, new_socket);
	_register.add_entry(sockfd, server);
	socket_set.add_socket(new_socket);
}

void ConnectionManager::add_socket(Server* server, ClientSocket* new_socket, SocketSet& socket_set)
{
	int sockfd = new_socket->sockfd();
	if (sockfd > _max_socket)
		_max_socket = sockfd;
	_register.add_entry(sockfd, new_socket);
	_register.add_entry(sockfd, server);
	socket_set.add_socket(new_socket);
}