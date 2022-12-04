#ifndef CGI_HPP
#define CGI_HPP

#include "unistd.h"
#include "Method.hpp"

#define PYTHON 1
#define PHP 2

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
		void		send_to_cgi(int pip_to_cgi[2], int pip_from_cgi[2]);
		void		recv_from_cgi(int pip_to_cgi[2], int pip_from_cgi[2], int pid);
		char 		**make_argv(int cgi_type); // DEFINE 1 = "python" 2 = "PHP"
		char		**make_env(int cgi_type); // DEFINE 1 = "python" 2 = "PHP"
		void		set_path();
		std::string read_answer(int fd);

	public:
		Cgi(std::string cgi_file, std::string query_string);
		~Cgi();
		void	execute_cgi_request();
		void	execute_python_cgi();	
};


#endif