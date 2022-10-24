#include "Rule.hpp"
#include "Rule.hpp"
#include<cstdlib>
Rule::Rule()
{
}

Rule::Rule(std::string ruleContent)
{
	_content = ruleContent;
	_pos = _content.begin();
	_last = _content.end();
}

Rule::Rule(const Rule& otherRule)
	: _content(otherRule.get_content())
{
	_pos = _content.begin();
	_last = _content.end();
}

Rule& Rule::operator=(const Rule& otherRule)
{
	if (this == &otherRule)
		return (*this);
	_content = otherRule.get_content();
	return (*this);
}
Rule::~Rule() {}


std::string::iterator Rule::begin() {
	return (_content.begin());
}

std::string::const_iterator Rule::begin()const {
	return (_content.begin());
}

std::string::iterator Rule::end() {
	return (_content.end());
}

std::string::const_iterator Rule::end()const {
	return (_content.end());
}

void Rule::append(std::string word)
{
	_content.append(" ");
	_content.append(word);
}

Operator Rule::next_operator()
{
	size_t	op_position;

//	std::cerr << "_content: " << _content << std::endl;
	op_position = _content.find_first_of(operator_list());
	if (op_position == std::string::npos)
		return (NONE);
	return (static_cast<Operator>(_content[op_position]));
}

size_t	Rule::operator_pos(Operator op)
{
	return (_content.find_first_of(static_cast<char>(op)));
}

std::string Rule::left_hand_side(size_t pos)
{
	std::string::iterator it;

	it = _content.begin();
	return(new_token(it, it + pos));
}

std::string Rule::nextWord(void)
{
	std::string::iterator start;
	std::string::iterator endWord;

	if (_pos == _last)
		return ("");
	endWord = start = _pos;
	if (std::isspace(*start) == true)
	{
		skip_while_space(endWord, _last);
		//return (" ");
	}
	skip_until_space(endWord, _last);
	//std::cerr << "_pos: " << *_pos << " token: " << std::string(start, endWord) << std::endl;
	return (new_token(start, endWord));
}
void Rule::advance(size_t nb)
{
	while (nb > 0)
	{
		if (std::isspace(*_pos) == false)
			nb--;
		_pos++;
	}
}
void Rule::clear_until_pos()
{
	if (_pos != _last)
	{
		_content.erase(_content.begin(), _pos);
	}
	else
		_content.clear();
	_pos = _content.begin();
	_last = _content.end();
	size_t i = 0;
	while (_pos != _last && std::isspace(*_pos) == true)
	{
		i++;
		_pos++;
	}
	
	if (i > 0)
	{
		i--;
		//_pos--;
		_content.erase(0, i);
	}
	_pos = _content.begin();
	_last = _content.end();
		
	//std::cerr << std::string(_content.begin(), _pos) << std::endl;
}

std::string::const_iterator Rule::get_pos(void)const
{
	return (_pos);
}

std::string::iterator Rule::get_pos(void)
{
	return (_pos);
}


std::string	Rule::get_next_string()
{
	size_t start;
	size_t end;

	start = _content.find_first_of('\"');
	end = _content.find_first_of('\"', start + 1);
	
	std::string::iterator it;
	//std::cerr << "->" <<_content << std::endl;
	it = _content.begin();
	//std::cerr << "next_string: " << *(it + start + 1) << std::endl;

	//std::cerr << "LOL: " << std::endl;
	return (std::string(_content.begin() + (start + 1), _content.begin() + end));
}


std::string	Rule::get_next_sequence(Operator op)
{
	char c;
	size_t start;
	size_t end;

	// std::cerr << _content << std::endl;
	c = static_cast<char>(op);
	start = _content.find_first_of(c);
	if (op == GROUPSTART)
		c = ')';
	else if (op == REPEATSTART)
		c = '}';
	end = _content.find_first_of(c, start + 1);
	if (end == std::string::npos)
		end = start;
	std::string::iterator it;
	//std::string::iterator it2;

	it = _content.begin();
	return (std::string( std::string(it + start + 1, it + end )));
}


std::string	Rule::get_next_group_sequence()
{
	size_t start;
	size_t end;

	// std::cerr << _content << std::endl;
	start = _content.find_first_of('(');
	end = _content.find_last_of(')');
	if (end == std::string::npos)
		end = start;
	std::string::iterator it;
	//std::string::iterator it2;

	it = _content.begin();
	return (std::string( std::string(it + start, it + (end + 1) )));
}


void Rule::eraseWord(size_t size)
{
	//std::cerr << "size: " << size << std::endl;
	_content.erase(0, size);
	_pos = _content.begin();
	_last = _content.end();
}

bool Rule::empty(void)
{
	return (_content.empty());
}

void Rule::set_content(std::string newContent)
{
	_content = newContent;
	_pos = _content.begin();
	_last = _content.end();
}

const std::string Rule::get_content(void)const
{
	return (_content);
}

std::string Rule::get_content(void)
{
	return (_content);
}