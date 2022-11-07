#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "../Socket/ClientSocket.hpp"
#include "../Server/Server.hpp"
#include <unistd.h>

class Client
{
	ClientSocket	*_socket;
	struct sockaddr_in _addresse;

	// Request			_request;
	// Answer			_answer;
	Server&			_server_connected;
private:
	void fill_addresse();

public:
	void open(int sockfd);
	void create_request();
	void create_answer();
	void send_answer();
	void close();

public:
	void new_socket(ClientSocket& socket);
	const struct sockaddr_in& get_addresse()const;
	struct sockaddr_in& get_addresse();
	ClientSocket& get_socket();
	ClientSocket& get_request();
	ClientSocket& get_answer();
	ClientSocket& get_server();

public:
	Client(int sockfd, Server& server);
	Client(ClientSocket& socket, Server& server);

};

#endif