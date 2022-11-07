#include "Webserv/Webserv.hpp"

int main(int argc, char *argv[])
{
	Webserv webserv;
	if (argc == 2)
		webserv.start(argv[1]);
}