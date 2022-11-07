#include "ServerSocket.hpp"

ServerSocket::ServerSocket(int family, int port, std::string ip)
{
	int yes = 1;
	_name.sin_family = family;
	_name.sin_port = htons(port);
	_name.sin_addr.s_addr = inet_addr(ip.c_str());
	this->socket();
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	this->bind();
}

ServerSocket::ServerSocket(const ServerSocket &origin)
{
	_name.sin_family = origin._name.sin_family;
	_name.sin_port = htons(origin._name.sin_port);
	_name.sin_addr.s_addr = origin._name.sin_addr.s_addr;
}

ServerSocket &ServerSocket::operator=(const ServerSocket & origin)
{ if (this == &origin)
	return (*this);
	_name.sin_family = origin._name.sin_family;
	_name.sin_port = htons(origin._name.sin_port);
	_name.sin_addr.s_addr = origin._name.sin_addr.s_addr;
	return (*this);
}


void ServerSocket::socket() {
	_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_fd == -1)
		throw(std::exception());
}

void ServerSocket::bind()
{
	if (::bind(_fd, (sockaddr *)(&_name), sizeof(_name)) == -1)
		throw (std::exception());
}

void ServerSocket::listen(int queue)
{
	if (::listen(_fd, queue) == -1)
		throw (std::exception());
}


int ServerSocket::accept()
{
	int client_socket;
	socklen_t addrlen;

	addrlen = sizeof(_name);
	client_socket = ::accept(_fd, (sockaddr *)(&_name), &addrlen);
	if (client_socket == -1)
		throw (std::exception());
	return (client_socket);
}

void ServerSocket::close() {
	if (::close(_fd) == -1)
		throw (std::exception());
}

int ServerSocket::sockfd() const {
	return (_fd);
}


struct sockaddr_in &ServerSocket::get_name()
{

}

const struct sockaddr_in &ServerSocket::get_name()const
{

}
bool ServerSocket::is_listening_port()const
{
	return (true);
}

ServerSocket::~ServerSocket() {}