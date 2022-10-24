#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "Request.hpp"
#include "Answer.hpp"
#include "Socket.hpp"
#include <vector>
#include "../config/Settings.hpp"

// #define PORT 8080

class Server {
	private:
		std::vector<Socket>		_listeningSockets;
		Settings				_settingsInfo;

	public:
		Server();
		~Server();

		void listen_connection();
		void handle_connection();
		void perror_exit(std::string str);
};

char	*ft_strjoin(char *s, char c);

#endif