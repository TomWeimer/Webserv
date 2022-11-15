#include "Answer.hpp"

AnswerBody::AnswerBody(Server *server, Request *request)
	: _server(server), _request(request) {}
AnswerBody::~AnswerBody() {}

std::string AnswerBody::obtain_body(AnswerHeader* header, AnswerStatus* status)
{
	_header = header;
	if (_server->no_error())
		select_method();
	else
		error_page(status);
	return (_body);
}

void AnswerBody::error_page(AnswerStatus* status)
{
	_body = _server->error_page();
	if (_body.empty() == true)
		new_error_page(status);
}

void AnswerBody::new_error_page(AnswerStatus* status)
{
	int			status_code;
	std::string result;

	status_code = _server->get_status_code();
	result.append("<!DOCTYPE html>\n<html>\n<head>\n\t<meta charset=\"UTF-8\">\n\t<title>webserv</title>\n</head>\n");
	result.append("<body>\n");
	result.append("\t<h1>" + status->message(status_code) + "</h1>\n");
	result.append("\t<p>Click <a href=\"/\">here</a> to return home.</p>\n");
	result.append("</body>\n</html>");
	
	_body.clear();
	_body = result;
	_header->add_header("Content-Length: " + NumberToString(_body.size()));
}


void AnswerBody::select_method()
{
	if (_request->_method == "GET")
		method_get();
	else if (_request->_method == "POST")
		method_post();
	else if (_request->_method == "DELETE")
		method_delete();
	else
		_server->set_status_code(405);
}

void AnswerBody::method_get()
{
	if (_server->is_valid_method("GET", _request->_location) == false)
		_server->set_status_code(405);
	else
	{
		if (_server->no_error())
			_server->process_get(_header, _body, _request->_target);
	}
}

void AnswerBody::method_post()
{
	if (_server->is_valid_method("POST", _request->_location) == false)
		_server->set_status_code(405);
	else
	{
		_body = _request->_body;
		if (_server->no_error())
			_server->process_post(_header, _body, _request->_target);
	}
}

void AnswerBody::method_delete()
{
	if (_server->is_valid_method("DELETE", _request->_location) == false)
		_server->set_status_code(405);
	else
	{
		if (_server->no_error())
			_server->process_delete(_header, _body, _request->_target);
	}
}
