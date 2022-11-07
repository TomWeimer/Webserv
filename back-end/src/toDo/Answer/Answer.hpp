#ifndef ANSWER_HPP
#define ANSWER_HPP
#include "../Server/Server.hpp"
#include "../Request/Request.hpp"

class Answer
{
private:
	Server *_server;
	Request *_request;
	std::vector<std::string> _header;
	std::string _body;
	std::string _answer;

public:
	Answer();
	~Answer();

	void set_server(Server *server);
	void set_request(Request *request);
	void create_status_line();
	void create_header();
	void create_body();
	void create_message();

	std::string message();
};

#endif