#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include <map>
#include <vector>
#include <iostream>
#include "../Server/Server.hpp"
#include "../ManageConnection/ManageConnection.hpp"
#include "../Parsing/Lexer/Lexer.hpp"

class Webserv
{
private:
	std::vector<Server*> _servers;
private:
	int find_number_of_server(std::vector<KeyWord> parsed_file);
	void create_list_of_servers(std::vector<KeyWord> server_info);
	void new_server(std::vector<KeyWord>& parsed_file);
	std::vector<KeyWord> server_config(std::vector<KeyWord>& parsed_file);
	std::vector<KeyWord> parse_config_file(const std::string& configFile);
	void create_servers(const std::string& configFile);

public:
	void start(const std::string& configFile);
	void stop();

public:
	Webserv();
	~Webserv();
};


// class ServerInfo
// {
// 	std::map<std::string, std::string> _server_name;
// 	std::map<std::string, Server *> _server;

// public:
// 	Server *find(const std::string &host, const int &port);
// 	Server *find(const int &port);
// 	Server *find(const std::string &host);
// 	void push_back(const Server *newServer);

// public:
// 	ServerInfo();
// 	~ServerInfo();
// };
#endif
