#include "Server/Server.hpp"

int main(int argc, char *argv[])
{
	std::string configFile;
	if (argc == 2)
		configFile = argv[1];
   
    Server server(configFile);
	server.start();
}