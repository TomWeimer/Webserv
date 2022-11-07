#include "Client.hpp"

Client::Client(ClientSocket& socket, Server& server)
	: _server_connected(server)
{
	_socket = &socket;
	fill_addresse();
}

void Client::new_socket(ClientSocket& socket)
{
	_socket = &socket;
}


void Client::fill_addresse()
{
	socklen_t addrlen = sizeof(_addresse);

	getsockname(_socket->sockfd(), reinterpret_cast<sockaddr*>(&_addresse), &addrlen);
}

void Client::close()
{
	::close (_socket->sockfd());
}

void Client::open(int sockfd)
{
	ClientSocket new_socket(sockfd);
	_socket = &new_socket;
}




struct sockaddr_in& Client::get_addresse()
{
	return (_addresse);
}

const struct sockaddr_in& Client::get_addresse()const
{
	return (_addresse);
}


// // Obtain the data received by the socket
// std::string ManageConnection::obtain_message_from_socket(int socketFd)
// {
// 	char *buf = (char *)calloc(200, sizeof(char));
// 	std::string fileStr;
// 	int nDataLength;

// 	while ((nDataLength = recv(socketFd, buf, 200, 0)) > 0)
// 	{
// 		fileStr.append(buf, nDataLength);
// 		if (buf[nDataLength - 1] == '\n')
// 			break;
// 		bzero((void *)buf, 200);
// 	}
// 	free(buf);
// 	return fileStr;
// }