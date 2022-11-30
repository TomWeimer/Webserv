#include "Server.hpp"

Server::Server(std::vector<KeyWord> tokens)
	: _status_code(0)
{
	serverConfiguration(tokens);
	init_server_sockets();
}

// Fill the settings of the server
void Server::serverConfiguration(std::vector<KeyWord> tokens)
{
	Config config(_info, _locationsList);
	config.server_configuration(tokens);

}

// Create the sockets of the server
void Server::init_server_sockets()
{
	for (size_t i = 0; i < _info.port.size(); i++)
	{
		ServerSocket *new_socket = new ServerSocket(AF_INET, _info.port[i], _info.host[i]);
		_server_sockets.push_back(*new_socket);
	}
}

// After receiving a new request the server analyze the content
// of the request and return a fitting answer to the client.
void  Server::handle_request(ClientSocket* client, Request* request)
{
	verify_request(*request);
	send_response(request, client);
}

void	Server::verify_request(Request& request)
{
	check_http(request);
	check_version(&request);
	check_method(&request);
	check_limit(&request);
	if (request.method == "GET")
		check_target(&request);
	
}

void	Server::check_http(Request& request)
{
	if (request.method.empty() || request.target.empty() || request.version.empty())
		set_status_code(400);
}


void	Server::check_version(Request *request)
{
	if (request->version == "HTTP/1.1" && request->host.empty() == true)
		set_status_code(400);
	else if (request->version == "HTTP/0.9" || request->version == "HTTP/2" || request->version == "HTTP/3")
		set_status_code(505);
}

void	Server::check_method(Request* request)
{
	std::string method = request->method;
	if (!( method == "GET" || method == "POST" || method == "DELETE"))
	{
		if ( method == "HEAD" || method == "PUT" || method == "CONNECT"
			|| method == "OPTIONS" || method == "TRACE" || method == "PATCH")
				set_status_code(501);
		else
			set_status_code(400);
	}
	if (is_allowed_method(request) == false)
		set_status_code(405);
}

void	Server::check_limit(Request* request)
{
	if (request->size_limit_reached == true)
		set_status_code(413);
}

bool	Server::is_allowed_method(Request* request)
{
	std::vector<std::string>::iterator	it;
	std::vector<std::string>*			allowed_methods;

	allowed_methods = &request->location->allowed_methods;
	for (it = allowed_methods->begin(); it != allowed_methods->end(); it++)
	{
		if (*it == request->method)
			return (true);
	}
	return (false);
	
}

void	Server::check_target(Request* request)
{
	if (isDir(request->target) == true)
	{
		if (request->location->autoindex != ON && search_index(request) == false)
		{
			std::cerr << "yes" << std::endl;
			set_status_code(403);
		}
	}
	if (file_exists(request->target) == false)
		set_status_code(404);

}

bool	Server::search_index(Request* request)
{
	std::vector<std::string> *index_names;
	std::vector<std::string>::iterator it;
	std::string copy_target;

	copy_target = request->target;
	if (request->location->index.empty() == false)
		index_names = &request->location->index;
	else
		index_names = &_info.index;
	for (it = index_names->begin(); it != index_names->end(); it++)
	{
		if (file_exists(copy_target + *it) == true)
		{
			request->target += *it;
			return (true);
		}
	}
	return (false);
}

void			Server::send_response(Request* request, ClientSocket* client)
{
	Response		response;
	ResponseMaker	responseMaker(&response, request, &_status_code);

	responseMaker.make_response();
	client->send(response.make_response());
}

size_t Server::size() {
	return (_server_sockets.size());
}

ServerSocket& Server::operator[](int index) {
	return (_server_sockets[index]);
}

void Server::set_status_code(int number) {
	if (_status_code == 0 || number == 0)
		_status_code = number;
}


Server::~Server()
{
	ServerSocket *to_delete;

	while (_server_sockets.empty() == false)
	{
		to_delete = &(_server_sockets.back());
		_server_sockets.pop_back();
		delete to_delete;
	}
}


