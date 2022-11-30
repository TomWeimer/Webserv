#ifndef REQUESTMAKER_HPP
#define	REQUESTMAKER_HPP
#include "Request.hpp"
#include "../../Server/Server.hpp"
#include "../../utils/Parsing/Lexer/Lexer.hpp"

class RequestMaker
{
private:
	typedef void (RequestMaker::*functor)(KeyWord &);
	typedef std::vector<KeyWord> tokens_t;
	typedef std::map<std::string, functor> map_header_t;

private:
	Request*		_request;
	Server*			_server;
	map_header_t	_assign;

public:
	RequestMaker(Server *server, Request *request);
	~RequestMaker();

public:
	void make_request(std::string request);

private:
	void		set_information();
	void		set_headers_token();
	void		assign_content(std::string request);
	tokens_t	parse_request();
	void		assign_header(tokens_t &tokens);
	void		assign_request_line(KeyWord &keyword);
	void		assign_target(KeyWord &keyword);
	void		assign_method(KeyWord &keyword);
	void		assign_version(KeyWord &keyword);
	void		assign_host(KeyWord &keyword);
	void		assign_chunked(KeyWord &keyword);
	void		assign_file_extention();
	BlockParams	*find_location(std::string target);
	void		expand_request();
	std::string	expand_redirection();
	std::string	expand_target();
	bool	header_is_too_large(KeyWord& keyword);
};
#endif