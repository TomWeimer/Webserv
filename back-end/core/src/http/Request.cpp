#include "../../include/http/Request.hpp"
#include "utils/Parsing/Lexer.hpp"


Request::Request(int socketFD, std::string fullRequest)
	: _http_lexer(Vocabulary("./back-end/conf/HTTP.ebnf")),
	_full_request(fullRequest), _socket_fd(socketFD)
{
	_body = obtain_request_body();
	parse_request();
}

std::string Request::obtain_request_body()
{
	size_t pos;

	pos = _full_request.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		_http_lexer.set_input(_full_request);
		return ("");
	}
	_http_lexer.set_input(_full_request.substr(0, pos - 1));
	return (_full_request.substr(pos + 3));
}

void Request::parse_request()
{
	_http_lexer.set_input(_full_request);
	_full_tokens = _http_lexer.lexeme();
	assign_tokens();
}

void Request::assign_tokens()
{
	KeyWord *actualToken;
	for (size_t i = 0; i < _full_tokens.size(); i++)
	{
		actualToken = &_full_tokens[i];
		
		if (actualToken->tokenType == "<request>")
			assign_request_tokens(actualToken);
		else if (actualToken->tokenType == "<Host>")
			assign_host_tokens(actualToken);
		else
			assign_other_tokens(actualToken);
	}
}

void Request::assign_request_tokens(KeyWord* request)
{
	_method = request->args[0].valueToken;
	_target = request->args[1].valueToken;
	_version = request->args[2].valueToken;	
}

void Request::assign_host_tokens(KeyWord* host)
{	
	_host = host->args[0].valueToken;
}

void Request::assign_other_tokens(KeyWord* other)
{
	if (other != NULL)
	{
		_header_field.push_back(*other);
	}
}


std::string Request::getRequestType(){
	return this->_method;
}

std::string Request::getRout(){
	std::cerr << _target << std::endl;
	return this->_target;
}


int	Request::getSocketFd(){
	return this->_socket_fd;
}

std::string	Request::getFullRequest(){
	return this->_full_request;
}

Request::~Request(){}
