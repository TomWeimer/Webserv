#ifndef ANSWERHEADER_HPP
#define ANSWERHEADER_HPP
#include <vector>
#include <iostream>

class AnswerHeader
{
	private:
		std::vector<std::string> _headers;

	public:
	AnswerHeader();
	~AnswerHeader();

	void add_header(std::string new_header);
	public:
	std::string obtain_headers();
};

#endif