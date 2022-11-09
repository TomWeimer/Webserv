#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Socket
{
	protected:
	int 				_fd;
	struct sockaddr_in	_name;

	private:
	virtual int sockfd() const = 0;
	virtual struct sockaddr_in& get_name() = 0;
	virtual const struct sockaddr_in& get_name()const = 0;

	public:
	Socket() {}
	Socket(int fd) : _fd(fd)
	{
		socklen_t addrlen = sizeof(_name);
		getsockname(_fd, reinterpret_cast<sockaddr*>(&_name), &addrlen);
	}
	
	Socket(int fd, struct sockaddr_in name)
		: _fd(fd), _name(name) {}
	
	virtual bool	is_listening_port()const		 = 0;
	virtual  ~Socket() {}
	
};

#endif