#ifndef ANSWER_HPP
#define ANSWER_HPP
#include "../Server/Server.hpp"
#include "../Request/Request.hpp"
#include "AnswerHeader.hpp"
#include "AnswerStatus.hpp"

class Answer
{
	private:
	Server*						_server;
	Request*					_request;
	std::vector<std::string>	_headerLists;
	char**						_cgi_env;

	std::string create_message();
	void        set_cgi_env(std::string method);

public:
	Answer(Server *server, Request *request);
	~Answer();
	std::string message();
	void	cgi_get_request();
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