#ifndef RULE_HPP
#define RULE_HPP
#include <iostream>
#include "utils.hpp"
#include <fstream>

class Rule
{
	private:
	std::string				_content;

	public:
	void		fill(std::string filename);
	std::string	nextWord(void);
	void		append(std::string content);
	void		eraseWord(size_t size);
	bool		empty();
	void set_content(std::string newContent);
	const std::string get_content(void)const;
	std::string get_content(void);

};

#endif