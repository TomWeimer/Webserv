#ifndef MANAGE_CONNECTION_HPP
#define MANAGE_CONNECTION_HPP
#include <map>
#include <vector>
#include "../Server/Server.hpp"
#include "../Socket/Server/ServerSocket.hpp"
#include "../Socket/Client/ClientSocket.hpp"

#include <sys/select.h>

class ConnectionManager
{
	// --------------------------------------------------------------------------------------
	// Nested class used to manage the sets of the function select
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
		void			clear();
		fd_set			*get_set();
		const fd_set	*get_set()const;
		int				is_in_set(int fd);
		void			remove_socket(int fd);
		void			add_socket(ClientSocket* newSocket);
		void			add_socket(ServerSocket* newSocket);
	};
	// -------------------------------------------------------------------------------------
	// Nested class used to keep trace of the servers and the sockets
	class Register
	{
	private:
		std::map<int, Socket *>			_sockets;
		std::map<int, Server *> 		_servers;
	
	public:
		Register();
		~Register();
	
	public:
		int			max();
		Server*		find_server(int sockfd);
		Socket*		operator[](int sockfd);
		void		erase_entry(int sockfd);
		void		add_entry(int sockfd, Socket* socket);
		void		add_entry(int sockfd, Server* server);
	};
// -----------------------------------------------------------------------------------------
private:
	struct timeval			_timeout;
	std::vector<Server*>*	_servers;
	Register				_register;
	int						_max_socket;
	SocketSet				_all_sockets;
	SocketSet				_sockets_ready;

public:
	ConnectionManager();
	~ConnectionManager();

public:
	void			start_servers(std::vector<Server*> &servers);

private:
	void			start_timeout(int second, int microsecond);
	void			start_all_servers();
	void			start_one_server(Server& actual_server);

public:
	void			manage_connections();

private:
	void 			obtain_sockets_used_by_clients();
	void			handle_new_connection_or_request();
	void			new_connection(int fd);
	ClientSocket*	new_client(int fd);
	void			new_request(int fd);
	void			close_connection(ClientSocket *socket, Server *server);
	void 			add_socket(Server* server, ClientSocket* new_socket, SocketSet& socket_set);
	void 			add_socket(Server* server, ServerSocket* new_socket, SocketSet& socket_set);
};
#endif