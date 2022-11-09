#include "Answer.hpp"

Answer::Answer(Server *server, Request *request)
{
	_server = server;
	_request = request;
}

Answer::~Answer(){}

std::string Answer::message()
{	
	return (create_message());
}

std::string Answer::create_message()
{
	AnswerBody		_body(_server, _request);
	AnswerStatus	_status;
	AnswerHeader	_header;

	std::string tmp;
	std::string rtn;

	tmp = _body.obtain_body(&_header);
	rtn = _status.obtain_status_line(_request->_version, _server->get_status_code());
	rtn += _header.obtain_headers();
	rtn += "\r\n";
	rtn += "\r\n";
	rtn += tmp;

	
	return (rtn);

}




