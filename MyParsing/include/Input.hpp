#ifndef INPUT_HPP
#define INPUT_HPP
#include <iostream>
#include "utils.hpp"
#include <fstream>

std::string 	createInputFromFile(std::string filename);


class Input
{
	private:
	std::string::iterator	_last;
	std::string::iterator	_pos;
	std::string				_content;

	public:
	Input();
	Input(std::string filename);
	Input(const Input& otherInput);
	Input& operator=(const Input& otherInput);
	~Input();

	private:
	void		createInputFromFile(std::string filename);
	void		openFile(std::ifstream& file, std::string& filename);
	std::string readFile(std::ifstream& file);

	public:
	std::string					next_line();

	std::string::iterator		begin();
	std::string::const_iterator begin()const;

	std::string::iterator		end();
	std::string::const_iterator end()const;
	std::string restLine();
	void advance(size_t nb);
	char peek();

	void		fill(std::string filename);
	std::string	nextWord(void);
	void		append(std::string);
	void		eraseWord(size_t size);
	bool		empty();
	void clear_until_pos();
	void set_content(std::string newContent);
	const std::string get_content(void)const;
	std::string get_content(void);
	void move_pos(size_t nb);

	std::string::const_iterator get_pos(void)const;
	std::string::iterator get_pos(void);
	

};

#endif