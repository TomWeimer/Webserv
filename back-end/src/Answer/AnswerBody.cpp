#include "Answer.hpp"

AnswerBody::AnswerBody(Server *server, Request *request)
	: _server(server), _request(request) {}
AnswerBody::~AnswerBody() {}

std::string AnswerBody::obtain_body(AnswerHeader* header)
{
	_header = header;
	select_method();
	return (_body);
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
