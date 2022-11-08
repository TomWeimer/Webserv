#ifndef SERVER_HPP
#define SERVER_HPP
#include "../Config/Config.hpp"
#include "../Socket/ServerSocket.hpp"

class Server
{
private:
	ServerBlock					_info;
	std::vector<LocationBlock>	_locationsList;
	std::vector<ServerSocket>			_server_sockets;

public:
	Server(std::vector<KeyWord> tokens);
	~Server();

private:
	void init_server_sockets();
	void serverConfiguration(std::vector<KeyWord> tokens);

public:
	int get_port(int i);
	ServerSocket &getSocket(int i);
	ServerBlock* get_server_info();
	std::vector<LocationBlock>& get_location_list();
	ServerSocket& operator[](int index);
	size_t size();
};
#endif
