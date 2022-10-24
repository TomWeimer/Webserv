#ifndef RULE_HPP
#define RULE_HPP
#include <iostream>
#include "utils.hpp"
#include <fstream>

class Rule
{
	private:
	std::string::iterator	_last;
	std::string::iterator	_pos;
	std::string				_content;

	public:
	Rule();
	Rule(std::string filename);
	Rule(const Rule& otherRule);
	Rule& operator=(const Rule& otherRule);
	~Rule();

	public:
	std::string					next_word();
	Operator					next_operator();
	size_t						operator_pos(Operator op);
	std::string					get_next_string();
	std::string					get_next_sequence(Operator op);
	void						clear_until_pos();
	std::string::iterator		begin();
	std::string::const_iterator begin()const;

	std::string::iterator		end();
	std::string::const_iterator end()const;

	void		advance(size_t nb);
	void		fill(std::string filename);
	std::string	nextWord(void);
	void		append(std::string);
	void		eraseWord(size_t size);
	bool		empty();
	void set_content(std::string newContent);
	const std::string get_content(void)const;
	std::string get_content(void);

	std::string::const_iterator get_pos(void)const;
	std::string::iterator get_pos(void);
	std::string left_hand_side(size_t pos);
	std::string	get_next_group_sequence();
};


#endif