#include "Request.hpp"

#include "Request.hpp"
Request::Request() {}

void Request::set_server(Server *server) {
	_server = server;
}

void Request::set_content(std::string request)
{
	size_t		pos;
	std::string body;

	pos = request.find("\r\n\r\n");
	_request = request.substr(0, pos + 1);
	_body = _request.substr(pos + 3);
}

void  Request::parse_request()
{
	std::vector<KeyWord> tokens;

	parse_config_file(tokens);
}

void Request::parse_config_file(std::vector<KeyWord>& tokens)
{
	Lexer lexer(Vocabulary(".tools/HTTP.ebnf"));
}

void  Request::check_validity()
{
	
}


Request::~Request() {}