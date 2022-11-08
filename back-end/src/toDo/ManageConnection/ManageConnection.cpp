#include "ManageConnection.hpp"
#include <utility>

// Construction of the class
ManageConnection::ManageConnection(std::vector<Server*>& servers)
	: _servers(servers), _max_socket(0)
{
	init_ManageConnection();
}

void ManageConnection::init_ManageConnection()
{
	fill_timeout(2, 500000);
	fill_servers_sockets();
}

void ManageConnection::fill_timeout(int second, int microsecond)
{
	_timeout.tv_sec = second;
	_timeout.tv_usec = microsecond;
}

void ManageConnection::fill_servers_sockets()
{

	for (size_t i = 0; i < _servers.size(); i++)
	{
		for (size_t j = 0; j < (*_servers[i]).size(); j++)
		{
			add_socket(&(*_servers[i]), &(*_servers[i])[j], _all_sockets);
			(*_servers[i])[j].listen(100);
		}
	}
	std::cerr << _servers.empty() << std::endl;
}

void	ManageConnection::server_connection()
{
	//bool		res;

    std::cout << "---------WAITNG FOR NEW CONNECTIONS... (timeout = 2.5 seconde) -----------" << std::endl;
    while (true) 
    {
		_sockets_ready = _all_sockets;
		obtain_sockets_ready_to_read();
		handle_new_connection_or_request();
    }
}



// Manage the different connection with the server by using select
void  ManageConnection::obtain_sockets_ready_to_read()
{
	int timeout;
	
	timeout = select(_max_socket + 1, _sockets_ready.get_set(), NULL, NULL, &_timeout); // write fd
	if (timeout < 0)
		exit(1);
	if (timeout == 0)
		std::cerr << "[ERROR] select() timeout\n" << std::endl; // removing clients
}

// Navigate through all the sockets and check if they received something
// If it is the case, it either add a new client or handle the request.
void ManageConnection::handle_new_connection_or_request()
{

	for (int fd = 0; fd <= _max_socket; fd++)
	{
		if (_sockets_ready.is_in_set(fd))
		{
			if (_register[fd]->is_listening_port() == true)
				create_new_client(fd);
			else
				handle_new_request(fd);
		}
	}
}



// If the client is a new one, we add it to the server
void ManageConnection::create_new_client(int socketFd)
{
	std::cerr << "listening port" << std::endl;
	Server			*server;
	ServerSocket	*listeningSocket;
	ClientSocket	*new_client;
	int				clientFd;

	std::cerr << "new client" << std::endl;
	listeningSocket =  dynamic_cast<ServerSocket *>(_register[socketFd]);
	clientFd = listeningSocket->accept();
	server = _register.find_server(socketFd);
	new_client = new ClientSocket(clientFd);

	add_socket(server, new_client, _all_sockets);
}



// After receiving a new request the server analyze the content
// of the request and return a fitting answer to the client.
void ManageConnection::handle_new_request(int socketFd)
{
	ClientSocket	*socket;
	//Request			*request;
	//Answer			*answer;
	Server 			*server;
	std::cerr << "new Request" << std::endl;
	socket = dynamic_cast<ClientSocket *>(_register[socketFd]);
	server = _register.find_server(socket->sockfd()); 
	receive_request(socket, server);
	//answer = &create_answer(request);
	//send_answer(socket, answer);
	_register.erase_entry(socket->sockfd());
	_all_sockets.remove_socket(socket->sockfd());
	socket->close();
	_max_socket = _register.max();
	
}

void ManageConnection::receive_request(ClientSocket *socket, Server *server)
{
	int		buffer_size;

	buffer_size = 1000;
	Request new_request(socket->sockfd(), socket->recv(buffer_size), server);

	Answer answer(&new_request, server);
	answer.sendAnswer();
	// new_request.set_server(server);
	// new_request.set_content(socket->recv(buffer_size));
	// new_request.parse_request();
	// new_request.check_validity();
	//return (new_request);
}


// Answer ManageConnection::create_answer(Request *request, Server *server)
// {
// 	Answer new_answer;

// 	new_answer.set_server(server);
// 	new_answer.set_request(request);
// 	new_answer.create_status_line();
// 	new_answer.create_header();
// 	new_answer.create_body();
// 	new_answer.create_message();
// 	return (new_answer);
// }

// void ManageConnection::send_answer(ClientSocket *socket, Answer *answer)
// {
// 	socket->send(answer->message());
// }

void ManageConnection::add_socket(Server* server, ServerSocket* new_socket, SocketSet& socket_set)
{
	int sockfd = new_socket->sockfd();
	if (sockfd > _max_socket)
		_max_socket = sockfd;
	_register.add_entry(sockfd, new_socket);
	_register.add_entry(sockfd, server);
	socket_set.add_socket(new_socket);
}

void ManageConnection::add_socket(Server* server, ClientSocket* new_socket, SocketSet& socket_set)
{
	int sockfd = new_socket->sockfd();
	std::cerr << sockfd << std::endl;
	if (sockfd > _max_socket)
		_max_socket = sockfd;
	_register.add_entry(sockfd, new_socket);
	_register.add_entry(sockfd, server);

	socket_set.add_socket(new_socket);
	std::cerr << "yo: " << _register[6]->is_listening_port() << std::endl;
}



// remove a socket from the set of select
void ManageConnection::remove_socket_from_set(int socketFd, SocketSet& socket_set)
{
	_clients.erase(socketFd);
	FD_CLR(socketFd, socket_set.get_set());
}

