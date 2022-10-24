#ifndef REQUEST_HPP
#define REQUEST_HPP

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
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>



class Request {
	private:
		int								_socketFd;
		std::string						_fullRequest;
		std::vector<std::string>		_tokens;
		std::string 					_requestType;
		std::string 					_rout;

	public:
		Request(int socketFD, std::string fullRequest);
		~Request();

		void		setRout();
		void		setRequestType();
		void		setFullRequest();
		void		setTokens();
		std::string getRout();
		std::string getRequestType();
		int			getSocketFd();
};

char	*get_file(int fd);

#endif