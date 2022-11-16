#ifndef CGI_HPP
#define CGI_HPP
#include "../Server/Server.hpp"
#include "../Request/Request.hpp"
#include "AnswerHeader.hpp"
#include "AnswerStatus.hpp"
#include "unistd.h"

class Cgi
{
	private:
		Server*								_server;
		Request*							_request;
		std::map<std::string, std::string>	_cgi_env;
		std::string							_cgi_header;
		std::string							_cgi_body;
		bool								_is_cgi;
		bool								_is_cgi_valid;

		void		check_cgi_validity();
		void        set_cgi_env();
		void 	    split_cgi_answer(std::string &answer);
		std::string get_file_extention();

	public:
		Cgi(Server *server, Request *request);
		~Cgi();
		std::string message();
		void	execute_cgi_request();
};


#endif