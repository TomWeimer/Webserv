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

	std::string create_message();

public:
	Answer(Server *server, Request *request);
	~Answer();
	std::string message();
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
		void new_error_page(AnswerStatus* status);
		void error_page(AnswerStatus* status);
		std::string obtain_body(AnswerHeader *header, AnswerStatus* status);
	};
#endif