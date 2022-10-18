#ifndef INPUT_HPP
#define INPUT_HPP
#include <iostream>
#include "utils.hpp"
#include <fstream>

std::string 	createInputFromFile(std::string filename);


class Input
{
	private:
	std::string				_content;

	public:
	void		fill(std::string filename);
	std::string	nextWord(void);
	void		append(std::string);
	void		eraseWord(size_t size);
	bool		empty();
	void set_content(std::string newContent);
	const std::string get_content(void)const;
	std::string get_content(void);

};

#endif