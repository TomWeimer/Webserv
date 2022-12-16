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

std::string	ClientSocket::recv(int body_size, bool &_limit_reached)
{
	std::string 		str;
	char	buffer[65536];

	::recv(_fd, buffer, 65536, 0);
	str = buffer;
	if (str.substr(str.find("\r\n\r\n") + 4).length() >= static_cast<size_t>(body_size)) {
		_limit_reached = true;
		// std::cerr << "enter\n";
	}
	// std::cout << str << std::endl;
	
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
