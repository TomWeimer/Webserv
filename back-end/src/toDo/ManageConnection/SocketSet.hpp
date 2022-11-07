#ifndef SOCKETSET_HPP
#define SOCKETSET_HPP
#include "../Socket/Socket.hpp"
#include <sys/select.h>

class SocketSet
{
private:
	fd_set _socket_set;

public:
	SocketSet();
	SocketSet(const SocketSet& other);
	SocketSet& operator=(const SocketSet& other);
	~SocketSet();

public:
	void clear();
	int		is_in_set(int fd);
	void add_socket(Socket& socket);
	void remove_socket(int fd);
	fd_set *get_set();
};

#endif