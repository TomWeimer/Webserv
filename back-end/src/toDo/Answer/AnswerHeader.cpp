#include "AnswerHeader.hpp"

AnswerHeader::AnswerHeader()
	: _headers() {}


AnswerHeader::~AnswerHeader() {}

std::string AnswerHeader::obtain_headers()
{
	std::vector<std::string>::iterator it;
	std::string header_field;

	for (it = _headers.begin(); it != _headers.end(); it++)
	{
		header_field += *it;
	}
	_headers.clear();
	return header_field;
}

void AnswerHeader::add_header(std::string new_header)
{
	_headers.push_back(new_header);
	new_header += "\r\n";
}

