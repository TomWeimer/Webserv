#include "Webserv.hpp"

Webserv::Webserv() {}

// Create the servers, and start the communications with the clients.
void Webserv::start(const std::string& configFile)
{
	ManageConnection manager(_servers);

	create_servers(configFile);
	while (true)
	{
		manager.server_connection();
	}
}

// Obtain the information from the config file and then create the servers.
void Webserv::create_servers(const std::string& configFile)
{
	std::vector<KeyWord>	parsed_file;

	parsed_file = parse_config_file(configFile);
	while (parsed_file.empty() == false)
		new_server(parsed_file);
}

// Parse the configFile to obtain the number of server, and their setting.
std::vector<KeyWord> Webserv::parse_config_file(const std::string& configFile)
{
	Lexer	lexer(".tools/webserv.ebnf", configFile);

	return (lexer.lexeme());
}

// Create the server and add it into _servers
void Webserv::new_server(std::vector<KeyWord>& parsed_file)
{
	_servers.push_back(Server(server_config(parsed_file)));
}


// obtain the information related to the server
std::vector<KeyWord> Webserv::server_config(std::vector<KeyWord>& parsed_file)
{
	bool							stop;
	int								symbol_nb;
	std::vector<KeyWord>			server_config;
	std::vector<KeyWord>::iterator	it;

	stop = false;
	symbol_nb = 0;
	for (it = parsed_file.begin(); stop == false; it++)
	{
		if (it->tokenType == "<server_start>" || it->tokenType == "<start>")
			symbol_nb++;
		if (symbol_nb == 1 && it->tokenType == "<end>")
			stop == true;
		if (it->tokenType == "<end>")
			symbol_nb--;
	}
	server_config.insert(server_config.begin(), parsed_file.begin(), it);
	return (server_config);
}