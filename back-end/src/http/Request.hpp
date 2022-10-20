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
#include <iomanip>


class Request {
	private:
		int				_socket_fd;
		char*			_full_request;
		std::string 	_request_type;
		std::string 	_rout;

	public:
		Request(int socketFD, char *fullRequest);
		~Request();

		void		setRout();
		void		setRequestType();
		void		setFullRequest();
		std::string getRout();
		std::string getRequestType();
		int			getSocketFd();
};

char	*get_file(int fd);

#endif