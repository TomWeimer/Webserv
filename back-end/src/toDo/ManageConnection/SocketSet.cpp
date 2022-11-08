#include "SocketSet.hpp"

SocketSet::SocketSet()
{
	FD_ZERO(&_socket_set);
}

SocketSet::SocketSet(const SocketSet& other)
	: _socket_set(*other.get_set()) {}

SocketSet& SocketSet::operator=(const SocketSet& other)
{
	if (this == &other)
		return (*this);
	this->_socket_set = *other.get_set();
	return (*this);
}

void SocketSet::add_socket(ServerSocket* newSocket)
{
	FD_SET(newSocket->sockfd(), &_socket_set);
}

void SocketSet::remove_socket(int sockfd)
{
	FD_CLR(sockfd, &_socket_set);
}

void SocketSet::add_socket(ClientSocket* newSocket)
{
	FD_SET(newSocket->sockfd(), &_socket_set);
}

// Return if the socket received data or not
int	SocketSet::is_in_set(int fd)
{
	return (FD_ISSET(fd, &_socket_set));
}

fd_set *SocketSet::get_set()
{
	return (&_socket_set);
}

const fd_set *SocketSet::get_set()const
{
	return (&_socket_set);
}

SocketSet::~SocketSet() {}