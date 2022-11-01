#ifndef ANSWER_HPP
#define ANSWER_HPP
#include "../Server.hpp"
#include "../../utils/utils.hpp"
#include "Request.hpp"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <cstdio>
#include <netinet/in.h>
#include <fstream>
#include <iomanip>



class Answer {
	private:
		class Server			*_server;
		std::string		_fullAnswer;
		std::string		_finalLocation;
		std::string		_header;
		std::string		_body;
		class Request			*_request;
		bool			_invalid_rout;

	public:
		Answer(Request	*request, Server* server);
		~Answer();

		void	setFullAnswer();
		void	setHeader();
		void	setBody(std::string bodyLocation);
		void	invalidRequest(std::string statusCode);
		void	deleteRequest();
		void	postRequest();
		void	getRequest();
		void	sendAnswer();
		void	checkRout(); //temporary
};


#endif