#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "Request.hpp"
#include "Answer.hpp"
#include "Socket.hpp"
#include <vector>
#include "utils/utils.hpp"
#include "config/Settings.hpp"

// #define PORT 8080

class Server {
	private:
		std::vector<Socket>		_listeningSockets;
		fd_set					_master;
		fd_set					_readFds;
		Settings				_settingsInfo;
		struct timeval			_timeout;

	public:
		Server();
		~Server();

		void 		listenConnection();
		void 		handleConnection();
		void		handleTimeout(int fdMax);
		void 		perror_exit(std::string str);
		void		acceptConnection(int socketFd, int & fdMax);
		void		handleRequest(int socketFd);
		std::string	recvMessage(int socketFd);
		void 		initFdset();
		void		initSocket();
		void 		socketOption(); //goal: remove the "adress already in use" error
		bool		isListener(int socketFd);
		int 		maxListenerFd();
		Socket 		*findListenerFd(int socketFd);
};

char	*ft_strjoin(char *s, char c);

#endif