#include "Answer.hpp"

AnswerBody::AnswerBody(Server *server, Request *request)
	: _server(server), _request(request) {}
AnswerBody::~AnswerBody() {}

std::string AnswerBody::obtain_body(AnswerHeader* header, AnswerStatus* status)
{
	_header = header;
	if (_server->no_error())
		select_method();
	else if (_request->_redirected != NONE)
		redirection_body();
	else
		error_page(status);
	return (_body);
}

void AnswerBody::redirection_body()
{
	int status_code;

	status_code = _server->get_status_code();
	if (_request->_redirected == PERMANENT && status_code != 301)
		new_error_page(_request->_location->redirection.text);
	else
	{
		_header->add_header("Location: " + _request->_location->redirection.text);
		_body.clear();
	}
}

void AnswerBody::error_page(AnswerStatus* status)
{
	int status_code;

	_body = _server->error_page();
	if (_body.empty() == true)
	{
		status_code = _server->get_status_code();
		new_error_page(status->message(status_code));
	}
}

void AnswerBody::new_error_page(std::string message)
{
	std::string result;

	result.append("<!DOCTYPE html>\n<html>\n<head>\n\t<meta charset=\"UTF-8\">\n\t<title>webserv</title>\n</head>\n");
	result.append("<body>\n");
	result.append("\t<h1>" + message + "</h1>\n");
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
