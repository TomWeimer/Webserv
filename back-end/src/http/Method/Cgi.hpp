#ifndef CGI_HPP
#define CGI_HPP

#include "unistd.h"
#include "Method.hpp"

class Cgi
{
	private:
		// Method								_method;	
		std::string							_cgi_file;
		std::string							_query;
		std::string							_path;
		// std::string							_file_extension;
		// std::map<std::string, std::string>	_cgi_env;
		// std::string							_cgi_header;
		// std::string							_cgi_body;
		// bool								_is_cgi;
		// bool								_is_cgi_valid;

		// void		check_cgi_validity();
		// void        set_cgi_env();
		void 	    split_cgi_answer(std::string &answer);
		// void		set_path();
		std::string read_answer(int fd);

	public:
		Cgi(std::string cgi_file, std::string query_string);
		~Cgi();
		void	execute_cgi_request();
		void	execute_python_cgi();	
};


#endif