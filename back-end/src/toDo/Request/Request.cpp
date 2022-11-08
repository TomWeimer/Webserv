#include "Request.hpp"

// #include "Request.hpp"
// Request::Request() {}

// void Request::set_server(Server *server) {
// 	_server = server;
// }

// void Request::set_content(std::string request)
// {
// 	size_t		pos;
// 	std::string body;

// 	pos = request.find("\r\n\r\n");
// 	_request = request.substr(0, pos + 1);
// 	_body = _request.substr(pos + 3);
// }

// void  Request::parse_request()
// {
// 	std::vector<KeyWord> tokens;

// 	parse_config_file(tokens);
// }

// void Request::parse_config_file(std::vector<KeyWord>& tokens)
// {
// 	Lexer lexer(Vocabulary(".tools/HTTP.ebnf"));
// }

// void  Request::check_validity()
// {
	
// }


// Request::~Request() {}


Request::Request(int socketFD, std::string fullRequest, Server *server)
	: _server(server),
	_http_lexer(Vocabulary(".tools/HTTP.ebnf")),
	_full_request(fullRequest), _socket_fd(socketFD)
{
	_body = obtain_request_body();
	parse_request();
	_valid = check_request_validity();
	
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
	assign_location_block();
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

void Request::assign_location_block()
{
	ServerBlock *server_info;

	server_info = _server->get_server_info();
	if (minimal_http_requirement() == true)
	{
		if (assign_redirection() == false)
			_actualBlock = server_info;
		else
			std::cerr << "in a redirection" << std::endl;
	}
	if (_actualBlock != NULL)
		_final_target = _actualBlock->root + _target;
	else
		_final_target = _target;
}

bool Request::assign_redirection()
{
	std::vector<LocationBlock>& locationList = _server->get_location_list();
	size_t						i;
	
	
	if (locationList.empty() == true)
		return (false);
	for (i = 0; i < locationList.size(); i++)
	{
		if (_target == locationList[i].path)
		{
			_actualBlock = &locationList[i];
			return (true);
		}
	}
	return (false);
}

bool		Request::minimal_http_requirement()
{
	if (_method.empty() == true || _target.empty() == true || _version.empty() == true)
	{
		_valid = false;
		return (false);
	}
	if (_version == "HTTP/1.1" && _host.empty() == true)
	{
		_valid = false;
		return (false);
	}
	return (true);
}

bool Request::check_request_validity()
{
	if (method_is_valid() == false || target_is_valid() == false)
		return (false);
	return (true);
}

bool Request::method_is_valid()
{
	std::vector<std::string>::iterator it;

	for (it = _actualBlock->allowed_methods.begin(); it != _actualBlock->allowed_methods.end(); it++)
	{
		std::cerr << "it: " << *it << std::endl;
		if (*it == _method)
		{
			_valid_method = true;
			return (true);
		}
	}
	_valid_method = false;
	std::cerr << "method_invalid:" << std::endl;
	return (false);
}

bool Request::target_is_valid()
{
	std::ifstream	file;

	file.open(_final_target.c_str());
	if (!file.is_open())
	{
		_valid_target = false;
		std::cerr << "target_invalid:" << std::endl;
		return (false);
	}
	else 
	{
		file.close();
		_valid_target = true;
		return (true);
	}
}


bool Request::is_valid()
{
	return (_valid);
}

std::string Request::getRequestType(){
	return this->_method;
}

std::string Request::getRout(){
	std::cerr << "target: " <<  _target << std::endl;
	std::cerr << "final target: " <<  _final_target << std::endl;
	std::cerr << (_actualBlock == NULL) << std::endl;
	if (_actualBlock != NULL)
	{
		std::cerr << "root: " <<  _actualBlock->root << std::endl;
	//	std::cerr << "allowed_method: " << _actualBlock->allowed_methods[0] << std::endl;
		std::cerr << (_actualBlock == NULL) << std::endl;
	}
	

	return this->_final_target;

}


int	Request::getSocketFd(){
	return this->_socket_fd;
}

std::string	Request::getFullRequest(){
	return this->_full_request;
}

Request::~Request(){}
