#include "ClientSocket.hpp"

ClientSocket::ClientSocket(int sockfd)
	: Socket(sockfd) {}

ClientSocket::ClientSocket(const ClientSocket& other)
 : Socket(other.sockfd(), other.get_name()) {}

ClientSocket& ClientSocket::operator=(const ClientSocket& other)
{
	if (this == &other)
		return (*this);
	_fd = other.sockfd();
	_name = other.get_name();
	return (*this);
}

std::string	ClientSocket::recv(int buffer_size)
{
	std::string 		str;
	char	buffer[buffer_size];

	::recv(_fd, buffer, buffer_size, 0);
	str = buffer;
	return (str);
}

void	ClientSocket::send(std::string response_message)
{
	::send(_fd, response_message.c_str(), response_message.size(), 0);
}

void	ClientSocket::close()
{
	if (::close(_fd) == -1)
		throw (std::exception());
}

int ClientSocket::sockfd() const
{
	return (_fd);
}

struct sockaddr_in &ClientSocket::get_name()
{
	return (_name);
}

const struct sockaddr_in &ClientSocket::get_name()const
{
	return (_name);
}

bool ClientSocket::is_listening_port()const
{
	return (false);
}

ClientSocket::~ClientSocket() {}
