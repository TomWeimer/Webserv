#ifndef HANDLE_CONNECTION_HPP
#define HANDLE_CONNECTION_HPP
#include "./Server.hpp"
#include "./http/Request.hpp"
#include "./http/Answer.hpp"
#include <time.h>
#include <sys/select.h>

class HandleConnection
{
private:
	class Server	*_server;
	fd_set	_current_sockets;
	fd_set	_ready_sockets;
	struct timeval _timeout;


public:
	HandleConnection(Server *server);

private:
	void init_select_args();
	void init_timeout(int seconds, int microseconds);
	void init_socket_set();

public:
	void init();
	void listenConnection();
	void handleConnection();

private:
	void close_connection(int& max_socket);
	bool manage_sockets_with_select(int& max_socket);

private:
	void handle_new_connection_or_request(int& max_socket);
	int		socket_is_ready(int i);
	void handle_new_connection(int socketFd, int &max_socket);
	void handle_new_request(int socketFd);
	void add_new_connection(Socket &client, int &max_socket);
	std::string obtain_message_from_socket(int socketFd);

private:
	void send_new_answer(Request *newRequest);
	void remove_socket_from_the_set(int socketFd);
};
#endif