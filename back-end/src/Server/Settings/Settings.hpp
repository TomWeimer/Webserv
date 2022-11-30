#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <iostream>
#include <map>
#include <vector>

#define MAX_URL_LENGTH 4096
#define MAX_LENGTH_HEADER 16000

enum status
{
	UNINITIALIZED = 0,
	PERMANENT = 1,
	TEMPORARY = 2,
	ON = 3,
	OFF = 4
};

struct Redirection 
{
	int			type;
	int			status_code;
	std::string text;
};

struct CGI_params
{
    std::string cgi_name;
    std::string cgi_extension;
};

struct BlockParams
{
	int									autoindex;
	int									body_limit;
	std::string							root;
	std::vector<std::string>			index;
	std::map<int, std::string>			error_pages;
	std::vector<std::string>			allowed_methods;
	Redirection							redirection;
	CGI_params                          cgi;


	BlockParams()
		:	autoindex(OFF), body_limit(10000), root(""), index(),
			error_pages(), allowed_methods(), cgi()
		{
			redirection.type = UNINITIALIZED;
		} 
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
	ServerBlock() : BlockParams()
	{
		autoindex = OFF;
		body_limit = 10000;
		index.push_back("index.html");
		allowed_methods.push_back("GET");
		allowed_methods.push_back("POST");
		allowed_methods.push_back("DELETE");
	} 
	
};
#endif