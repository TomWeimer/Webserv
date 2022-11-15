#include "Request.hpp"

RequestHandler::RequestHandler(Server *server, Request* request)
	:  _request(request), _server(server)
{
	_request->_location = _server->get_server_info();
	_request->_redirected = false;
	insert_headers();
}

void RequestHandler::insert_headers()
{
	_assign.insert(std::make_pair("<request>", &RequestHandler::request_line));
	_assign.insert(std::make_pair("<Host>", &RequestHandler::host));
}

void RequestHandler::analyze_request(std::string request)
{
	std::vector<KeyWord> tokens;

	assign_content(request);
	tokens = parse_request();
	assign_header(tokens);
	std::cerr << "CGI: " << _request->_location->cgi.cgi_extension << std::endl;
	expand_request();
	verify_request();
}

void RequestHandler::assign_content(std::string request)
{
	size_t pos;

	pos = request.find("\r\n\r\n");
	_request->_line = request.substr(0, pos);
	_request->_body = request.substr(pos + 3);
}

std::vector<KeyWord> RequestHandler::parse_request()
{
	Lexer	lexer(Vocabulary("./back-end/.tools/HTTP.ebnf"));

	lexer.set_input(_request->_line);
	return (lexer.lexeme());
}

void RequestHandler::assign_header(std::vector<KeyWord>& tokens)
{
	std::vector<KeyWord>::iterator it;

	for (it = tokens.begin(); it != tokens.end(); it++)
	{
		if (_assign.find(it->tokenType) != _assign.end())
			(this->*_assign[it->tokenType])(*it);
	}
}

void RequestHandler::expand_request()
{
	_request->_location = _server->find_location(_request->_target);
	if (_request->_location->redirection.type != NONE)
		_request->_target = expand_redirection();
	_request->_target = expand_target();
}

std::string RequestHandler::expand_redirection()
{
	int			status_code;
	std::string redirection;

	_request->_redirected = _request->_location->redirection.type;
	if (_request->_redirected == TEMPORARY)
		status_code = 302;
	else
		status_code = _request->_location->redirection.status_code;
	_server->set_status_code(status_code);
	return ("");
}

std::string RequestHandler::expand_target()
{
	if (_request->_target.empty() == true)
		return ("");
	return (_server->obtain_final_target(_request->_location, _request->_target));
}


void RequestHandler::verify_request()
{
	if (_request->_target.empty() == true || _request->_method.empty() == true || _request->_version.empty() == true)
		_server->set_status_code(400);
	verify_method();
	verify_target();
	verify_version();
}

void RequestHandler::verify_method()
{
	std::string& method = _request->_method;

	if ( method == "GET" || method == "POST" || method == "DELETE")
		return ;
	else if ( method == "HEAD" || method == "PUT" || method == "CONNECT"
			|| method == "OPTIONS" || method == "TRACE" || method == "PATCH")
			_server->set_status_code(501);
	else
		_server->set_status_code(400);
}

void RequestHandler::verify_target()
{
	if (_server->is_valid_target(_request->_target, _request->_location) == false)
		_server->set_status_code(404);
	std::cerr << "target index: " << _request->_target << std::endl;
}


void RequestHandler::verify_version()
{
	if (_request->_version == "HTTP/1.1" && _request->_host.empty() == true)
		_server->set_status_code(400);
	else if (	_request->_version == "HTTP/0.9"
			||	_request->_version == "HTTP/2"
			||	_request->_version == "HTTP/3")
		_server->set_status_code(505);
}

void RequestHandler::request_line(KeyWord &keyword)
{
	target(keyword);
	method(keyword);
	version(keyword);
}

void RequestHandler::target(KeyWord &keyword) {
	_request->_target = keyword.args[1].valueToken;
}

void RequestHandler::method(KeyWord &keyword) {
	_request->_method = keyword.args[0].valueToken;
}

void RequestHandler::version(KeyWord &keyword) {
	_request->_version = keyword.args[2].valueToken;
}

void RequestHandler::host(KeyWord &keyword) {
	_request->_host = keyword.args[0].valueToken;
}

RequestHandler::~RequestHandler() {}