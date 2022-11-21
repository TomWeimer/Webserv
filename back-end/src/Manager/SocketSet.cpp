#include "ConnectionManager.hpp"

ConnectionManager::SocketSet::SocketSet()
{
	this->clear();
}

void ConnectionManager::SocketSet::add_socket(ServerSocket* newSocket)
{
	FD_SET(newSocket->sockfd(), &_socket_set);
}

void ConnectionManager::SocketSet::add_socket(ClientSocket* newSocket)
{
	FD_SET(newSocket->sockfd(), &_socket_set);
}

int	ConnectionManager::SocketSet::is_in_set(int fd)
{
	return (FD_ISSET(fd, &_socket_set));
}

void ConnectionManager::SocketSet::remove_socket(int sockfd)
{
	FD_CLR(sockfd, &_socket_set);
}

void ConnectionManager::SocketSet::clear()
{
	FD_ZERO(&_socket_set);
}

fd_set *ConnectionManager::SocketSet::get_set()
{
	return (&_socket_set);
}

const fd_set *ConnectionManager::SocketSet::get_set()const
{
	return (&_socket_set);
}

ConnectionManager::SocketSet::SocketSet(const SocketSet& other)
	: _socket_set(*other.get_set()) {}

ConnectionManager::SocketSet& ConnectionManager::SocketSet::operator=(const SocketSet& other)
{
	if (this == &other)
		return (*this);
	this->_socket_set = *other.get_set();
	return (*this);
}

ConnectionManager::SocketSet::~SocketSet() {}