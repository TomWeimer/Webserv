#ifndef CONFIG_HPP
#define CONFIG_HPP
#include "../../utils/utils.hpp"
#include "../Settings/Settings.hpp"
#include "../../utils/Parsing/Lexer/Lexer.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>

class Config
{
	typedef void (Config::*recipe_t)(std::vector<Token> &);
	typedef std::string key_t;

private:
	ServerBlock 				&_server_params;
	std::vector<LocationBlock>	&_locationsList;
	BlockParams					*_block;
	std::map<key_t, recipe_t>	_assign;

public:
	Config(ServerBlock &_info, std::vector<LocationBlock> &locationList);

private:
	void create_tokens_recipes();
	void new_entry(std::string, recipe_t);
	void cgi(std::vector<Token> &);
	void end(std::vector<Token> &);
	void root(std::vector<Token> &);
	void host(std::vector<Token> &);
	void port(std::vector<Token> &);
	void index(std::vector<Token> &);
	void socket(std::vector<Token> &);
	void listen(std::vector<Token> &);
	void redirect(std::vector<Token> &);
	void rewrite(std::vector<Token> &);
	void location(std::vector<Token> &);
	void autoindex(std::vector<Token> &);
	void error_pages(std::vector<Token> &);
	void server_name(std::vector<Token> &);
	void client_limit(std::vector<Token> &);
	void server_start(std::vector<Token> &);
	void allow_methods(std::vector<Token> &);

public:
	void server_configuration(std::vector<KeyWord> listToken);

private:
	bool is_invalid(std::vector<KeyWord> listToken);
	void start_configuration(std::vector<KeyWord> listToken);
	BlockParams *obtain_actual_block(std::string tokenType);
	void assign_token(KeyWord &keyword);
	void assign_socket(std::string host, std::string port);
	std::string format_host(std::string host);

public:
	~Config();
};
#endif