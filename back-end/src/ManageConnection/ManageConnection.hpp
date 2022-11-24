#ifndef MANAGE_CONNECTION_HPP
#define MANAGE_CONNECTION_HPP
#include <map>
#include "../Answer/Answer.hpp"
#include "../Request/Request.hpp"

#include "../Socket/ServerSocket.hpp"
#include "../Socket/ClientSocket.hpp"
#include "SocketSet.hpp"
#include "Register.hpp"
#include "../Server/Server.hpp"

class ManageConnection
{

	class Register
	{
	private:
		std::map<int, Socket *>			_sockets;						// key: sockfd	 value: socket
		std::map<int, Server *> 		_servers;				// key: sockfd	 value: server
	
	public:
		void		init();
		void		add_entry(int sockfd, Socket* socket);
		void		add_entry(int sockfd, Server* server);
		void		erase_entry(int sockfd);
		void		add_listening_socket(int sockfd, Socket &socket, Server& server);
		Server*		find_server(int sockfd);
		bool		is_registered(int sockfd);
		int			max();
		std::map<int, Socket *>	getSocket();
		void		setSocket(std::map<int, Socket *> &other);
		Socket		*operator[](int sockfd);
		~Register();
	};
	

private:
	Server*					_actual_server;

	struct timeval			_timeout;
	std::vector<Server*>&	_servers;
	int _max_socket;
	Register				_register;
	SocketSet				_sockets_ready;
	SocketSet				_all_sockets;
	

public:
	ManageConnection(std::vector<Server*> &servers);
	void server_connection();

private:
	Request receive_request(ClientSocket *socket);
	Answer create_answer(Request *request);
	void init_ManageConnection();
	void fill_timeout(int second, int microsecond);
	void fill_servers_sockets();
	void  obtain_sockets_ready_to_read();
	void handle_new_connection_or_request();
	int socket_is_ready_to_read(int fd);
	void remove_all_client();
	void add_socket(Server* server, ClientSocket* new_socket, SocketSet& socket_set);
	void add_socket(Server* server, ServerSocket* new_socket, SocketSet& socket_set);

	void create_new_client(int socketFd);
	void handle_new_client(int socketFd);
	void handle_new_request(int socketFd);
	void remove_socket_from_set(int socketFd, SocketSet& socket_set);
	Answer create_answer(Request *request, Server *server);
	void receive_request(ClientSocket *socket, Server *server);
	void send_answer(ClientSocket *socket, Answer *answer);
};

// class HandleConnection
// {
// private:
// 	class Server *_server;
// 	fd_set _current_sockets;
// 	fd_set _ready_sockets;
// 	struct timeval _timeout;

// public:
// 	HandleConnection(Server *server);

// private:
// 	void init_select_args();
// 	void init_timeout(int seconds, int microseconds);
// 	void init_socket_set();

// public:
// 	void init();
// 	void listenConnection();
// 	void handleConnection();

// private:
// 	void close_connection(int &max_socket);
// 	bool manage_sockets_with_select(int &max_socket);

// private:
// 	void handle_new_connection_or_request(int &max_socket);
// 	int socket_is_ready(int i);
// 	void handle_new_connection(int socketFd, int &max_socket);
// 	void handle_new_request(int socketFd);
// 	void add_new_connection(Socket &client, int &max_socket);
// 	std::string obtain_message_from_socket(int socketFd);

// private:
// 	void send_new_answer(Request *newRequest);
// 	void remove_socket_from_the_set(int socketFd);
// };
#endif