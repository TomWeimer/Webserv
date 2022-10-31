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
#include "utils/Parsing/Lexer.hpp"
#include <vector>

class Request
{
	private:
		Lexer					_http_lexer;
		std::string				_full_request;
		std::vector<KeyWord>	_full_tokens;
		std::string				_method;
		std::string				_target;
		std::string				_version;
		std::string				_host;
		std::vector<KeyWord>	_header_field;
		std::string				_body;
		int						_socket_fd;


	public:
		Request(int socketFD, std::string fullRequest);
		~Request();
	private:
		std::string	obtain_request_body();
		void		parse_request();
		void		assign_tokens();
		void 		assign_request_tokens(KeyWord* request);
		void 		assign_host_tokens(KeyWord* request);
		void 		assign_other_tokens(KeyWord* request);
		void		check_request_validity();

		
	
	public:
		std::string	getFullRequest();
		int			getSocketFd();
		std::string getRout();
		std::string getRequestType();
};

#endif