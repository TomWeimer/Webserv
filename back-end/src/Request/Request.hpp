#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Parsing/Lexer/Lexer.hpp"
#include "../Server/Server.hpp"
#include <vector>

struct Request
{
public:
	std::string					_line;
	std::string					_method;
	std::string					_target;
	std::string					_version;
	std::string					_host;
	std::vector<KeyWord>		_unknown;
	std::string					_body;
	BlockParams*				_location;
	int							_redirected;
	bool						_is_cgi; //
};

class RequestHandler
{
private:
	typedef void (RequestHandler::*functor)(KeyWord &);
	typedef std::vector<KeyWord> tokens_t;
	typedef std::map<std::string, functor> map_header_t;

private:
	Request*		_request;
	Server*			_server;
	map_header_t	_assign;

public:
	RequestHandler(Server *server, Request *request);
	~RequestHandler();

public:
	void analyze_request(std::string request);

private:
	void		insert_headers();
	void		assign_content(std::string request);
	tokens_t	parse_request();
	void		assign_header(tokens_t &tokens);
	void		request_line(KeyWord &keyword);
	void		target(KeyWord &keyword);
	void		method(KeyWord &keyword);
	void		cgi(KeyWord &keyword);
	void		version(KeyWord &keyword);
	void		host(KeyWord &keyword);
	void		expand_request();
	std::string	expand_redirection();
	std::string	expand_target();
	void		verify_request();
	void		verify_target();
	void		verify_method();
	void		verify_version();
	void		verify_cgi();
};

#endif