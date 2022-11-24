#include "RequestMaker.hpp"

// This class parses the request and assigns it's content in the Request struct.
RequestMaker::RequestMaker(Server *server, Request* request)
	:  _request(request), _server(server)
{
	set_information();
	set_headers_token();
}

// set default values
void RequestMaker::set_information()
{
	_request->location = _server->get_server_info();
	_request->redirected = false;
}

// add the tokens and their function into the map _assign
void RequestMaker::set_headers_token()
{
	_assign.insert(std::make_pair("<request>", &RequestMaker::assign_request_line));
	_assign.insert(std::make_pair("<Host>", &RequestMaker::assign_host));
}

void RequestMaker::make_request(std::string request)
{
	std::vector<KeyWord> tokens;

	assign_content(request);
	tokens = parse_request();
	assign_header(tokens);
	expand_request();
}

// separate the body from the rest of the request
void RequestMaker::assign_content(std::string request)
{
	size_t pos;

	pos = request.find("\r\n\r\n");
	std::cerr << "request: " << request << std::endl;
	_request->line = request.substr(0, pos);
	_request->body = request.substr(pos + 3);
}

// parse the request into tokens
std::vector<KeyWord> RequestMaker::parse_request()
{
	Lexer	lexer(Vocabulary("./back-end/.tools/HTTP.ebnf"));

	lexer.set_input(_request->line);
	return (lexer.lexeme());
}

// If a token match with one found in the map _assign we call it's function.
void RequestMaker::assign_header(std::vector<KeyWord>& tokens)
{
	std::vector<KeyWord>::iterator it;

	for (it = tokens.begin(); it != tokens.end(); it++)
	{
		if (_assign.find(it->tokenType) != _assign.end())
			(this->*_assign[it->tokenType])(*it);
	}
}

// token: <request>
// role: Contain the method, the target and the version of the request
void RequestMaker::assign_request_line(KeyWord &keyword)
{
	if (header_is_too_large(keyword) == true)
		_server->set_status_code(431);
	assign_target(keyword);
	assign_method(keyword);
	assign_version(keyword);
	std::cerr << "request: " << _request->method << " " << _request->target << " " << _request->version << std::endl;
}

// subtoken: <target>
void RequestMaker::assign_target(KeyWord &keyword) {
	_request->target = keyword.args[1].valueToken;
	if (_request->target.size() > MAX_URL_LENGTH)
		_server->set_status_code(414);
}

// subtoken: <method>
void RequestMaker::assign_method(KeyWord &keyword) {
	_request->method = keyword.args[0].valueToken;
}

// subtoken: <version>
void RequestMaker::assign_version(KeyWord &keyword) {
	_request->version = keyword.args[2].valueToken;
}

// token: <Host>
// role: Contains the host of the request is a requirement since the version HTTP/1.1
void RequestMaker::assign_host(KeyWord &keyword) {
	if (header_is_too_large(keyword) == true)
		_server->set_status_code(431);
	_request->host = keyword.args[0].valueToken;
}

// Assign the corresponding locationBlock to the request and
//	then expand the request's informations with the ones of the locationBlock
void RequestMaker::expand_request()
{
	_request->location = find_location(_request->target);
	if (_request->location->redirection.type != UNINITIALIZED)
		_request->target = expand_redirection();
	_request->target = expand_target();
	std::cerr << "request: " << _request->method << " " << _request->target << " " << _request->version << std::endl;
}

BlockParams *RequestMaker::find_location(std::string target)
{
	
	std::string location_path;
	std::string target_request;
	std::vector<LocationBlock>::iterator it;
	std::vector<LocationBlock>& location_list = _server->get_location_list();

	for (it = location_list.begin(); it != location_list.end(); it++)
	{
		location_path = it->root;
		location_path += it->path;
		target_request = it->root;
		target_request += target;
		if (std::strncmp(target_request.c_str(),  location_path.c_str(), location_path.size()) == 0)
			return &(*it);
	}
	return (_server->get_server_info());
}

// If the locationBlock redirect the target, 
// it can be either a temporary or a permanent redirection (keywords: rewrite or return)
std::string RequestMaker::expand_redirection()
{
	int			status_code;
	std::string redirection;

	_request->redirected = _request->location->redirection.type;
	if (_request->redirected == TEMPORARY)
		status_code = 302;
	else
		status_code = _request->location->redirection.status_code;
	_server->set_status_code(status_code);
	return ("");
}

// Expand the target with the root of the LocationBlock
std::string RequestMaker::expand_target()
{
	if (_request->target.empty() == true)
		return ("");
	return (_request->location->root + _request->target);
}

bool RequestMaker::header_is_too_large(KeyWord& keyword)
{
	size_t max_size = 0;
	std::vector<Token>::iterator it;

	for (it = keyword.args.begin(); it != keyword.args.end(); it++)
	{
		max_size += it->valueToken.size();
		if (max_size > MAX_LENGTH_HEADER)
			return (true);
	}
	return (false);
}




RequestMaker::~RequestMaker() {}