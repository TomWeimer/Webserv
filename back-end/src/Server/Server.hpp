#ifndef SERVER_HPP
#define SERVER_HPP
#include "../http/Response/ResponseMaker.hpp"
#include "./Settings/Settings.hpp"
#include "./Config/Config.hpp"
#include "../Socket/Server/ServerSocket.hpp"
#include "../Socket/Client/ClientSocket.hpp"
#include "../http/Request/Request.hpp"
#include "../http/Response/Response.hpp"


class Server
{
private:
	ServerBlock					_info;
	std::vector<LocationBlock>	_locationsList;
	std::vector<ServerSocket>	_server_sockets;
	int							_status_code;

public:
	int							get_status_code() const 	{ return _status_code; }
	int*							get_ptr_status_code() { return &_status_code; }
	ServerBlock*				get_server_info()			{ return &_info; }
	std::vector<LocationBlock>& get_location_list()			{ return _locationsList; }

public:
	Server(std::vector<KeyWord> tokens);
	~Server();

private:
	void init_server_sockets();
	void serverConfiguration(std::vector<KeyWord> tokens);

public:
	void handle_request(ClientSocket* client, Request* request);

private:
	
	bool	search_index(Request* request);
	void	check_target(Request* request);
	bool	is_allowed_method(Request* request);
	void	check_method(Request* request);
	void	check_http(Request& request);
	void	check_version(Request *request);
	void	check_limit(Request* request);
	void	send_response(Request *request, ClientSocket *client);

public:
void	verify_request(Request &request);
	void set_status_code(int number);
public:
	ServerSocket &operator[](int index);
	size_t size();
};

#endif
