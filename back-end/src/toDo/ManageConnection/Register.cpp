#include "ManageConnection.hpp"


// class Register
// 	{
// 	private:
// 		std::map<int, Socket &>			_sockets;						// key: sockfd	 value: socket
// 		std::map<int, Server &> 		_default_server;				// key: sockfd	 value: server
// 		std::map<std::string, Server&>	_hostname_server;				// key: hostname value: server
	
// 	public:
// 		void		init();
// 		void		add_entry(int sockfd, Socket &socket_ref);
// 		bool		is_registered(int sockfd);
// 		int			max();
// 		Socket	&operator[](int sockfd);
// 	};

void ManageConnection::Register::init()
{
}

void ManageConnection::Register::add_entry(int sockfd, Server& server)
{
	_servers.insert(std::make_pair(sockfd, server));
}

void ManageConnection::Register::add_entry(int sockfd, Socket &socket)
{
	_sockets.insert(std::make_pair(sockfd, &socket));
}

void ManageConnection::Register::erase_entry(int sockfd)
{
	if (_sockets.find(sockfd) != _sockets.end())
		_sockets.erase(sockfd);
	if (_servers.find(sockfd) != _servers.end())
		_servers.erase(sockfd);
}

Server&	 ManageConnection::Register::find_server(int sockfd)
{
	Socket &socket = _sockets.find(sockfd)->second;
	return (_servers[sockfd]);
}



bool		ManageConnection::Register::is_registered(int sockfd)
{
	return (_sockets.find(sockfd) != _sockets.end());
}

Socket&	ManageConnection::Register::operator[](int sockfd)
{
	return (_sockets[sockfd]);
}

int ManageConnection::Register::max()
{
	return (_sockets.rbegin()->first);
}