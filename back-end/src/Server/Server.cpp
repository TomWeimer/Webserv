#include "Server.hpp"

Server::Server(std::string configFile)
{
	serverConfiguration(configFile);
	init_server_sockets();
}

void Server::serverConfiguration(std::string configFile)
{
	if (configFile.empty() == true)
	{
		Config defaultConfiguration(_info, _locationsList);
		return ;
	}
	Lexer lexer("back-end/.tools/webserv.ebnf", "back-end/.config/webserv.conf");
	//std::cerr <<  lexer.lexeme();
	Config customConfiguration(_info, _locationsList, lexer.lexeme());
	//Config defaultConfiguration(_info, _locationsList);
	return ;
}

void Server::init_server_sockets()
{
	for (size_t i = 0; i < _info.port.size(); i++)
	{
		Socket *new_socket = new Socket(true, _info.port[i]);
		_server_sockets.push_back(*new_socket);
	}
}

void Server::start()
{
	HandleConnection ServerConnection(this);

	ServerConnection.listenConnection();
	ServerConnection.handleConnection();
}


bool Server::isServerSocket(int socketFd)
{
	for (size_t i = 0; i < _server_sockets.size(); i++)
	{
		if (socketFd == _server_sockets[i].getSocketFd())
			return true;
	}
	return false;
}

Socket *Server::find_sockfd(int socketFd)
{
	for (size_t i = 0; i < sockets_number(); i++)
	{
		if (socketFd == _server_sockets[i].getSocketFd())
			return &(_server_sockets[i]);
	}
	return &(_server_sockets[0]);
}

int Server::obtain_biggest_socket_fd()
{
	int max_socket;

	max_socket = _server_sockets[0].getSocketFd();
	for (size_t i = 0; i < sockets_number(); i++)
	{
		if (_server_sockets[i].getSocketFd() > max_socket)
			max_socket = _server_sockets[i].getSocketFd();
	}
	return (max_socket);
}

void Server::socketOption()
{ // remove "adress already in use" error msg
	int yes = 1;

	for (size_t i = 0; i < _server_sockets.size(); i++)
	{
		if (setsockopt(this->_server_sockets[i].getSocketFd(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
		{ // remove "adress already in use" error msg
			perror("setsockopt");
			exit(1);
		}
	}
}



int Server::getSocketFd(int i)
{
	return (_server_sockets[i].getSocketFd());
}

Socket &Server::getSocket(int i)
{
	return (_server_sockets[i]);
}

int Server::get_port(int i)
{
	return (_info.port[i]);
}

ServerBlock *Server::get_server_info()
{
	return (&_info);
}

std::vector<LocationBlock>& Server::get_location_list()
{
	return (_locationsList);
}

size_t Server::sockets_number()
{
	return (_server_sockets.size());
}

void Server::perror_exit(std::string str)
{
	perror(str.c_str());
	exit(EXIT_FAILURE);
}

Server::~Server()
{
	std::cout << "server destructor called" << std::endl;
	return;
}