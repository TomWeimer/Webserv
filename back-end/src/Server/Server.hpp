#ifndef SERVER_HPP
#define SERVER_HPP
#include "./config/Config.hpp"
#include "./socket/Socket.hpp"
#include "./HandleConnection.hpp"

class Server
{
private:
	ServerBlock					_info;
	std::vector<LocationBlock>	_locationsList;
	std::vector<Socket>			_server_sockets;

public:
	Server(std::string ConfigFile = "");
	~Server();

private:
	void init_server_sockets();
	void serverConfiguration(std::string configFile);

public:
	void start();
	void perror_exit(std::string str);
	void socketOption();
	size_t sockets_number();
	bool isServerSocket(int socketFd);
	Socket *find_sockfd(int socketFd);
	int obtain_biggest_socket_fd();

public:
	int getSocketFd(int i);
	int get_port(int i);
	Socket &getSocket(int i);
	ServerBlock* get_server_info();
	std::vector<LocationBlock>& get_location_list();
};
#endif