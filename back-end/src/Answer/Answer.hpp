#ifndef ANSWER_HPP
#define ANSWER_HPP
#include "../Server/Server.hpp"
#include "../Request/Request.hpp"
#include "AnswerHeader.hpp"
#include "AnswerStatus.hpp"
#include "unistd.h"

class Answer
{
	private:
	Server*						_server;
	Request*					_request;
	std::vector<std::string>	_headerLists;
	char**						_cgi_env; // we could make a cgi class
	std::string					_cgi_header;
	std::string					_cgi_body;
	std::string					_cgi_status;

	std::string create_message();
	void        set_cgi_env();
	void 	    split_cgi_answer(std::string &answer);
	void		send_to_cgi(int pip_to_cgi[2], int pip_from_cgi[2]);
	void		receive_from_cgi(int pip_to_cgi[2], int pip_from_cgi[2], int pid);	

public:
	Answer(Server *server, Request *request);
	~Answer();
	std::string message();
	void	execute_cgi_request();
	void	cgi_post_request();
};



class AnswerBody
	{
		private:
			Server *_server;
			Request *_request;
			std::string _body;
			AnswerHeader *_header;

		public:
		AnswerBody(Server *server, Request *request);
		~AnswerBody();
		void method_get();
		void method_post();
		void method_delete();
		void select_method();
		std::string obtain_body(AnswerHeader *status);
	};
#endif