#include "ConnectionManager.hpp"

ConnectionManager::Register::Register() {}

int ConnectionManager::Register::max()
{
	return (_sockets.rbegin()->first);
}

 void ConnectionManager::Register::add_entry(int sockfd, Server* server)
{
	_servers.insert(std::make_pair(sockfd, server));
}

void ConnectionManager::Register::add_entry(int sockfd, Socket* socket)
{
	_sockets.insert(std::make_pair(sockfd, socket));
}

Server*	 ConnectionManager::Register::find_server(int sockfd)
{
	return (_servers[sockfd]);
}

void ConnectionManager::Register::erase_entry(int sockfd)
{
	if (_sockets.find(sockfd) != _sockets.end())
		_sockets.erase(sockfd);
	if (_servers.find(sockfd) != _servers.end())
		_servers.erase(sockfd);
}

Socket*	ConnectionManager::Register::operator[](int sockfd)
{
	return (_sockets[sockfd]);
}

ConnectionManager::Register::~Register() {}