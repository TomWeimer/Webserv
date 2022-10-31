#ifndef ANSWER_HPP
#define ANSWER_HPP

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
#include <netinet/in.h>
#include <fstream>
#include <iomanip>


class Answer {
	private:
		std::string		_fullAnswer;
		Request			*_request;
		bool			_invalid_rout;

	public:
		Answer(Request	*request);
		~Answer();

		void	setFullAnswer();
		void	sendAnswer();
		std::string NumberToString ( size_t Number );
};


#endif