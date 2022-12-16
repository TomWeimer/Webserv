#include "Server/Server.hpp"
#include "Manager/ConnectionManager.hpp"

std::vector<KeyWord> obtain_server_config(std::vector<KeyWord>& parsed_file)
{
	bool							stop;
	int								symbol_nb;
	std::vector<KeyWord>			server_config;
	std::vector<KeyWord>::iterator	it;

	stop = false;
	symbol_nb = 0;
	for (it = parsed_file.begin(); stop == false; it++)
	{
		if (it->tokenType == "<server_start>" || it->tokenType == "<location>")
			symbol_nb++;
		if (symbol_nb == 1 && it->tokenType == "<end>")
			stop = true;
		if (it->tokenType == "<end>")
			symbol_nb--;
	}
	server_config.insert(server_config.begin(), parsed_file.begin(), it);
	parsed_file.erase(parsed_file.begin(), it);
	return (server_config);
}
void	new_server(std::vector<KeyWord>& parsed_file, std::vector<Server*>& servers)
{
	static int i = 0;
	try
	{
		Server *new_server = new Server(obtain_server_config(parsed_file));
		servers.push_back(new_server);
		std::cerr << "Server" << i << ": created" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "Server" << i << ": not created" << std::endl;
		std::cerr << e.what() << std::endl;
	}
	i++;
}

std::vector<KeyWord> parse_config_file(const std::string& configFile)
{
	std::string ebnf_path;
	char *test_folder;

	test_folder = getenv("TEST_FOLDER");
	if (test_folder == NULL)
		ebnf_path = "./back-end/.tools/GrammarFiles/webserv.ebnf";
	else
	{
		ebnf_path = test_folder;
		ebnf_path += "/../back-end/.tools/GrammarFiles/webserv.ebnf";
	}
	

	Lexer	lexer(ebnf_path.c_str(), configFile);
	return (lexer.lexeme());
}

void create_servers(const std::string& configFile, std::vector<Server*>& servers)
{
	std::vector<KeyWord>	parsed_file;

	parsed_file = parse_config_file(configFile);
	while (parsed_file.empty() == false)
		new_server(parsed_file, servers);
}



int main(int argc, char *argv[])
{
	ConnectionManager		*manager = new ConnectionManager();
	std::vector<Server*>	servers;

	if (argc == 2)
		create_servers(argv[1], servers);
	else
		create_servers("back-end/.config/default.conf", servers);
	if (servers.empty() == false)
	{
		manager->start_servers(servers);
		manager->manage_connections();
	}
}







