#include "Config.hpp"

// This class takes as argument references to locations and info, variable inside the server class
// The purpose of this class is to take the array of tokens and assigns their value inside the
// variable mentionned before
Config::Config(ServerBlock& info, std::vector<LocationBlock>& locations, std::vector<KeyWord> listToken)
	: _serverInfo(info), _locationsList(locations), _serverBlock(true)
{
	// std::cerr << listToken << std::endl;
	if (listToken.empty() == false && listToken[0].tokenType == "<server_start>")
	{
		fill_token();
		fill_config(listToken);
	}
}

// Initialize the the tokens functions
void Config::fill_token()
{
	_fill_token.insert(std::make_pair("<server_start>", &Config::server_start));
	_fill_token.insert(std::make_pair("<location>", &Config::location));
	_fill_token.insert(std::make_pair("<end>", &Config::end));
	_fill_token.insert(std::make_pair("<listen>", &Config::listen));
	_fill_token.insert(std::make_pair("<socket>", &Config::socket));
	_fill_token.insert(std::make_pair("<host>", &Config::host));
	_fill_token.insert(std::make_pair("<port>", &Config::port));
	_fill_token.insert(std::make_pair("<root>", &Config::root));
	_fill_token.insert(std::make_pair("<index>", &Config::index));
	_fill_token.insert(std::make_pair("<cgi>", &Config::cgi));
	_fill_token.insert(std::make_pair("<server_name>", &Config::server_name));
	_fill_token.insert(std::make_pair("<allow_methods>", &Config::allow_methods));
	_fill_token.insert(std::make_pair("<client_limit>", &Config::client_limit));
	_fill_token.insert(std::make_pair("<error_pages>", &Config::error_pages));
	_fill_token.insert(std::make_pair("<autoindex>", &Config::autoindex));
	_fill_token.insert(std::make_pair("<return>", &Config::return_redirection));
	_fill_token.insert(std::make_pair("<rewrite>", &Config::rewrite_redirection));
	_fill_token.insert(std::make_pair("<cgi>", &Config::cgi));
}

// loop in the listToken, and call the coresponding function
void Config::fill_config(std::vector<KeyWord> listToken)
{
	std::vector<KeyWord>::iterator first;

	first = listToken.begin();
	for(; first != listToken.end(); first++) 
	{
		//std::cerr << *first << std::endl;
		if (_fill_token.find(first->tokenType) != _fill_token.end())
			(this->*_fill_token[first->tokenType])(*first);
	}

}

// Return if the token is inside a server block or inside a location block.
BlockParams* Config::obtainBlock()
{
	if (_serverBlock == true)
		return(&_serverInfo);
	else
		return(&_locationsList.back());
}

// first token of the server indicate the beginning of a server
void Config::server_start(KeyWord keyword)
{
	(void)keyword;

	_serverInfo.body_limit = 0;
	_serverInfo.root = "";
}

// Indicate the start of a new location, create a new locationBlock
// and add his modifier and path
void Config::location(KeyWord keyword)
{
	LocationBlock *actualLocation;

	_serverBlock = false;
	_locationsList.push_back(LocationBlock());
	actualLocation = &_locationsList.back();
	actualLocation->modifier = keyword.args[0].valueToken;
	actualLocation->path = keyword.args[1].valueToken;	
	actualLocation->body_limit = 0;
	actualLocation->root = "";
}

// Indicate the end of a location block or the end of the serverBlock
void Config::end(KeyWord keyword)
{
	(void)keyword;
	if (_serverBlock == false)
		_serverBlock = true;
}

// indicate a new port, socket or host to listen from, call the corresponding function with the name of their token.
void Config::listen(KeyWord keyword)
{
	//std::cerr << keyword << std::endl;
	return ((this->*_fill_token[keyword.args[0].tokenType])(keyword));
}


// The argument of the listen keyword is a socket (eg: 127.000.000.1:80)
// Obtain the host and the port from the socket
void Config::socket(KeyWord keyword)
{
	Token		*arg;
	std::string tmp;
	std::string host;
	size_t		pos;

	arg = &keyword.args[0];
	pos = arg->valueToken.find_first_of(':');
	host = arg->valueToken.substr(0, pos);
	if (host == "localhost")
		host = "127.0.0.1";
	_serverInfo.host.push_back(host);
	tmp = arg->valueToken.substr(pos + 1);
	_serverInfo.port.push_back((int)std::strtol(tmp.c_str(), NULL, 10));
	std::cerr << _serverInfo.host.back() << std::endl;
	std::cerr << _serverInfo.port.back() << std::endl;
}

// The argument of listen is an host (eg: 0.0.0.0)
// Add the host to the list, and the default port 80
void Config::host(KeyWord keyword)
{
	std::string tmp;
	tmp = keyword.args[0].valueToken;
	if (tmp == "localhost")
		tmp = "127.0.0.1";
	_serverInfo.host.push_back(tmp);
	_serverInfo.port.push_back(80);
}

// The argument of listen is a port (eg: 42)
// Add the default host "0.0.0.0" to the list, and the correct port number
void Config::port(KeyWord keyword)
{
	std::string tmp;

	tmp = keyword.args[0].valueToken;
	_serverInfo.port.push_back((int)std::strtol(tmp.c_str(), NULL, 10));
	_serverInfo.host.push_back( "0.0.0.0");
}

// Add the root to the corresponding codeBlock
void Config::root(KeyWord keyword)
{
	BlockParams *actualBlock;

	actualBlock = obtainBlock();
	actualBlock->root = keyword.args[0].valueToken;
}

// Add the indexes to the corresponding codeBlock
void Config::index(KeyWord keyword)
{

	BlockParams *actualBlock;

	actualBlock = obtainBlock();
	for (std::vector<Token>::iterator it = keyword.args.begin(); it != keyword.args.end(); it++)
	{
		actualBlock->index.push_back(it->valueToken);
	}
}


// Add the server_name to the server
void Config::server_name(KeyWord keyword)
{
	_serverInfo.name.push_back(keyword.args[0].valueToken);
}


// Add the allowed methods to the corresponding codeBlock
void Config::allow_methods(KeyWord keyword)
{
	BlockParams *actualBlock;

	actualBlock = obtainBlock();
	for (std::vector<Token>::iterator it = keyword.args.begin(); it != keyword.args.end(); it++)
	{
		actualBlock->allowed_methods.push_back(it->valueToken);
	}
}

// Add the client_limit to the corresponding codeBlock
void Config::client_limit(KeyWord keyword)
{
	std::string tmp;
	BlockParams *actualBlock;

	actualBlock = obtainBlock();
	tmp = keyword.args[0].valueToken;
	actualBlock->body_limit = (int)std::strtol(tmp.c_str(), NULL, 10);
}

// Add the error_pages to the corresponding codeBlock
void Config::error_pages(KeyWord keyword)
{

	BlockParams *actualBlock;
	std::string	path;
	int			error_nb;

	actualBlock = obtainBlock();
	for (std::vector<Token>::reverse_iterator it = keyword.args.rbegin(); it != keyword.args.rend(); it++)
	{
		if (it == keyword.args.rbegin())
			path = it->valueToken;
		else
		{
			error_nb = static_cast<int>(std::strtol(it->valueToken.c_str(), NULL, 10));
			actualBlock->error_pages.insert(std::make_pair(error_nb, path));
		}
	}
}

void Config::return_redirection(KeyWord keyword)
{
	BlockParams *actualBlock;

	actualBlock = obtainBlock();
	actualBlock->redirection.type = PERMANENT;
	actualBlock->redirection.status_code =  static_cast<int>(std::strtol(keyword.args[0].valueToken.c_str(), NULL, 10));
	actualBlock->redirection.text = keyword.args[1].valueToken;
}

void Config::rewrite_redirection(KeyWord keyword)
{
	BlockParams *actualBlock;

	actualBlock = obtainBlock();
	actualBlock->redirection.type = TEMPORARY;
	actualBlock->redirection.text = keyword.args[0].valueToken;
}

// Add the client_limit to the corresponding codeBlock
void Config::autoindex(KeyWord keyword)
{
	std::string tmp;
	BlockParams *actualBlock;

	actualBlock = obtainBlock();
	if (keyword.args[0].valueToken == "on")
		actualBlock->autoindex = ON;
	else
		actualBlock->autoindex = OFF;
}

void Config::cgi(KeyWord keyword)
{
    BlockParams *actualBlock;

    actualBlock = obtainBlock();
    actualBlock->cgi.cgi_extension = keyword.args[0].valueToken;
    actualBlock->cgi.cgi_name = keyword.args[1].valueToken;
}

Config::~Config() {}