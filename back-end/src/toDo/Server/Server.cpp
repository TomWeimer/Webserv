#include "Server.hpp"

Server::Server(std::vector<KeyWord> tokens)
{
	serverConfiguration(tokens);
	init_server_sockets();
}

void Server::serverConfiguration(std::vector<KeyWord> tokens)
{
	Config configuration(_info, _locationsList, tokens);
}

void Server::init_server_sockets()
{
	for (size_t i = 0; i < _info.port.size(); i++)
	{
		ServerSocket *new_socket = new ServerSocket(AF_INET, _info.port[i], _info.host[i]);
		_server_sockets.push_back(*new_socket);
	}
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

size_t Server::size()
{
	return (_server_sockets.size());
}

ServerSocket& Server::operator[](int index)
{
	return (_server_sockets[index]);
}

Server::~Server()
{
	std::cout << "server destructor called" << std::endl;
	return;
}