#ifndef CONFIG_HPP
#define CONFIG_HPP
#include "../utils/Parsing/Lexer/Lexer.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>

struct BlockParams
{
	bool						autoindex;
	int							body_limit;
	std::string					root;
	std::vector<std::string>	index;
	std::map<int, std::string>	error_pages;
	std::vector<std::string>	allowed_methods;
};

struct LocationBlock : public BlockParams
{
	std::string path;
	std::string modifier;
};

struct ServerBlock : public BlockParams
{
	std::vector<int> port;
	std::vector<std::string> host;
	std::vector<std::string> name;
};

class Config
{
public:
	typedef void (Config::*functor)(KeyWord keyword);

private:
	ServerBlock&					_serverInfo;
	std::vector<LocationBlock>&		_locationsList;
	std::map<std::string, functor>	_fill_token;
	bool							_serverBlock;

private:
	BlockParams *obtainBlock();
	void fill_token();
	void fill_config(std::vector<KeyWord> listToken);

	void server_start(KeyWord keyword);
	void end(KeyWord keyword);
	void listen(KeyWord keyword);
	void socket(KeyWord keyword);
	void host(KeyWord keyword);
	void port(KeyWord keyword);
	void root(KeyWord keyword);
	void index(KeyWord keyword);
	void location(KeyWord keyword);
	void server_name(KeyWord keyword);
	void allow_methods(KeyWord keyword);
	void client_limit(KeyWord keyword);
	void error_pages(KeyWord keyword);
	void autoindex(KeyWord keyword);

public:
	Config(ServerBlock &_info, std::vector<LocationBlock> &locationList, std::vector<KeyWord> listToken);
	~Config();
};
#endif