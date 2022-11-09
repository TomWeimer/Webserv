#ifndef ANSWER_STATUS_HPP
#define ANSWER_STATUS_HPP
#include <map>
#include <iostream>

class AnswerStatus
{
	private:
		std::map<int, std::string> _status_code;
		void fill_error_message();

	public:
		void		add_header(std::string header);
		std::string	obtain_status_line(std::string version, int status_code);
		AnswerStatus();
		~AnswerStatus();
};

#endif