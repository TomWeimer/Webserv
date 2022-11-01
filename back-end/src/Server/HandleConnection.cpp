#include "HandleConnection.hpp"

// Construction of the class
HandleConnection::HandleConnection(Server *server)
	: _server(server)
{
	_timeout.tv_sec = 2;
	_timeout.tv_usec = 500000;
	init_socket_set();
}

// Create the sets of socket used by select to managing connection
void HandleConnection::init_socket_set()
{
	FD_ZERO(&_current_sockets);
	FD_ZERO(&_ready_sockets);
	for (size_t i = 0; i < _server->sockets_number(); i++)
	{
		FD_SET(_server->getSocketFd(i), &_current_sockets);
	}
}

// Start to listening on the server ports
void HandleConnection::listenConnection()
{
	// TODO - check how to define and handle the max number of connection
	for (size_t i = 0; i < _server->sockets_number(); i++)
	{
		std::cout << "listening on PORT:  " << _server->get_port(i) << std::endl;
		_server->getSocket(i).listenPort(100);
	}
}

// MAIN function to handling the connection of the server,
// it add a new connection or handle a new request.
void	HandleConnection::handleConnection(){ // siege -b http://localhost:8080/front-end/html/index.html
	int		fdMax;
	bool		res;
	fdMax = _server->obtain_biggest_socket_fd(); 

	_server->socketOption(); //remove recurent "adress already in use" error msg
    std::cout << "---------WAITNG FOR NEW CONNECTIONS... (timeout = 2.5 seconde) -----------" << std::endl;
    while(1) 
    {
		this->_ready_sockets = this->_current_sockets;
		res = manage_sockets_with_select(fdMax);
		if (res == false)
		{ //timeout
			this->close_connection(fdMax);
			continue;
		}
		handle_new_connection_or_request(fdMax);
    }
}

// void HandleConnection::handleConnection() // siege -b http://localhost:8080/front-end/html/index.html
// {
// 	int max_socket;
// 	bool timeout;

	
// 	max_socket = _server->obtain_biggest_socket_fd();
// 	_server->socketOption();
// 	std::cout << "---------WAITNG FOR NEW CONNECTIONS... (timeout = 2.5 seconde) -----------" << std::endl;
// 	while (true)
// 	{
// 		//std::cerr << "lol" << std::endl;
// 		_ready_sockets = _current_sockets;
// 		timeout = manage_sockets_with_select(max_socket);
// 		if (timeout == true)
// 		{
// 			//std::cerr << "HELLO" << std::endl;
// 			close_connection(max_socket);
// 		}
// 		handle_new_connection_or_request(max_socket);
// 	}
// }

// Manage the different connection with the server by using select
bool HandleConnection::manage_sockets_with_select(int& max_socket)
{
	int timeout;
	
	timeout = select(max_socket + 1, &_ready_sockets, NULL, NULL, &_timeout);
	if (timeout < 0)
		_server->perror_exit("select");
	return (timeout != 0);
}

// If the time until the end of a connection is over,
// then the server end the connection with the client
void HandleConnection::close_connection(int& max_socket)
{
	for (int i = 0; i <= max_socket; i++)
	{
		if (i > 2 && _server->isServerSocket(i) == false)
		{
			close(i);
			FD_CLR(i, &_current_sockets);
		}
	}
}

// Navigate through all the sockets and check if they received something
// If it is the case, it either add a new client or handle the request.
void HandleConnection::handle_new_connection_or_request(int& max_socket)
{
	for (int i = 0; i <= max_socket; i++)
	{
		if (socket_is_ready(i))
		{
			if (_server->isServerSocket(i))
				handle_new_connection(i, max_socket);
			else
				handle_new_request(i);
		}
	}
}

// Return if the socket received data or not
int HandleConnection::socket_is_ready(int i)
{
	return (FD_ISSET(i, &_ready_sockets));
}

// If the client is a new one, we add it to the server
void HandleConnection::handle_new_connection(int socketFd, int &max_socket)
{
	Socket client(_server->find_sockfd(socketFd), false);

	if (client.getSocketFd() == -1)
		perror("new connection");
	else
	{
		add_new_connection(client, max_socket);
	}
}

// To create a new client we add his socket to the set of socket managed by select
void HandleConnection::add_new_connection(Socket &client, int &max_socket)
{
	int sockfd;

	sockfd = client.getSocketFd();
	FD_SET(sockfd, &_current_sockets);
	if (sockfd > max_socket)
		max_socket = client.getSocketFd();
}

// After receiving a new request the server analyze the content
// of the request and return a fitting answer to the client.
void HandleConnection::handle_new_request(int socketFd)
{
	std::string requestContent;

	std::cout << "newRequest" << std::endl;
	requestContent = obtain_message_from_socket(socketFd);
	Request newRequest(socketFd, requestContent, _server);
	send_new_answer(&newRequest);
	remove_socket_from_the_set(socketFd);
}


// Obtain the data received by the socket
std::string HandleConnection::obtain_message_from_socket(int socketFd)
{
	char *buf = (char *)calloc(200, sizeof(char));
	std::string fileStr;
	int nDataLength;

	while ((nDataLength = recv(socketFd, buf, 200, 0)) > 0)
	{
		fileStr.append(buf, nDataLength);
		if (buf[nDataLength - 1] == '\n')
			break;
		bzero((void *)buf, 200);
	}
	free(buf);
	return fileStr;
}

// send a new answer to the client
void HandleConnection::send_new_answer(Request *newRequest)
{
	Answer answer(newRequest, _server);
	answer.sendAnswer();
}

// remove a socket from the set of select
void HandleConnection::remove_socket_from_the_set(int socketFd)
{
	close(socketFd);
	FD_CLR(socketFd, &this->_current_sockets);
}
