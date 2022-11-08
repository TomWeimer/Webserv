#include "Socket.hpp"


Socket::Socket() {}
Socket::Socket(int fd) : _fd(fd)
{
	socklen_t addrlen = sizeof(_name);
	getsockname(_fd, reinterpret_cast<sockaddr*>(&_name), &addrlen);
}

Socket::Socket(int fd, struct sockaddr_in name)
		: _fd(fd), _name(name) {}
