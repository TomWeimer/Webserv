#include "SocketSet.hpp"

void SocketSet::add_socket(Socket& newSocket)
{
	FD_SET(newSocket.sockfd(), &_socket_set);
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