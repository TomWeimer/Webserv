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

// #define PORT 8080

class Server {
	private:
		int 				_server_fd;
		struct sockaddr_in	_address;
		int					_addrlen;
		int					_PORT;

	public:
		Server();
		// Server(const Server &other);
		~Server();
		//Server & operator=(Server const &rhs);

		void listen_connection();
		void handle_connection();
		void perror_exit(std::string str);
};

char	*ft_strjoin(char *s, char c);

#endif