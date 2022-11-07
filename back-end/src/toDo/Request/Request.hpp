#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../utils/Parsing/Lexer/Lexer.hpp"
#include "../Server/Server.hpp"
#include <vector>

struct RequestLine
{
	std::string				_method;
	std::string				_target;
	std::string				_version;
};

struct RequestHeader
{
	std::string				_host;
	std::vector<KeyWord>	_unknown;
};

class Request
{
	private:
		Server					*_server;
		std::string				_request;
		RequestLine				_request_line;
		RequestHeader			_request_header;
		std::string				_body;
		bool					_valid;


	public:
		Request();
		~Request();
	public:
	void set_server(Server* server);
	void set_content(std::string request);
	void parse_request();
	void check_validity();

	private:
	void parse_config_file(std::vector<KeyWord>& tokens);
		
};

#endif