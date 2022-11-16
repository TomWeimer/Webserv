#include "ServerSocket.hpp"
#include <sstream>

int ipToInt(int first, int second, int third, int fourth)
{
    return ((first << 24) | (second << 16) | (third << 8) | (fourth));
}

in_addr_t ip_to_address(std::string ip)
{
	// std::cerr << "ip: " << ip << std::endl;
	std::stringstream s(ip);
	int first, second, third, fourth; //to store the 4 ints
	int internet_address;
	
	char dot; //to temporarily store the '.'
	s >> first >> dot >> second>> dot >> third>> dot >> fourth;
	internet_address = ipToInt(first, second, third, fourth);
	return (static_cast<in_addr_t>(ntohl(internet_address)));
}

ServerSocket::ServerSocket(int family, int port, std::string ip)
{
	int yes = 1;
	_name.sin_family = family;
	_name.sin_port = htons(port);
	_name.sin_addr.s_addr = ip_to_address(ip);
	this->socket();
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	// std::cerr << "newClient" << std::endl;
	this->bind();
	// std::cerr << "newClient" << std::endl;
}

ServerSocket::ServerSocket(const ServerSocket &origin)
	: Socket(origin)
{
	_name.sin_family = origin._name.sin_family;
	_name.sin_port = htons(origin._name.sin_port);
	_name.sin_addr.s_addr = ntohl(origin._name.sin_addr.s_addr);
}

ServerSocket &ServerSocket::operator=(const ServerSocket & origin)
{ 
	if (this == &origin)
		return (*this);
	_name.sin_family = origin._name.sin_family;
	_name.sin_port = htons(origin._name.sin_port);
	_name.sin_addr.s_addr = ntohl(origin._name.sin_addr.s_addr);;
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
	{
		std::cerr << "bind failed" << std::endl;
		throw (std::exception());
	}
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
	return (_name);
}

const struct sockaddr_in &ServerSocket::get_name()const
{
	return (_name);
}

bool ServerSocket::is_listening_port()const
{
	return (true);
}

ServerSocket::~ServerSocket() {}