#include "Rule.hpp"

void Rule::append(std::string word)
{
	_content.append(" ");
	_content.append(word);
}

void Rule::fill(std::string ruleContent)
{
	_content = ruleContent;
}

std::string Rule::nextWord(void)
{
	std::string::iterator endWord;
	std::string::iterator endContent;

	endWord = _content.begin();
	endContent = _content.end();
	if (std::isspace(_content[0]) == true)
		skip_and_erase_while_space(_content, endWord, endContent);
	skip_until_space(endWord, endContent);
	return (std::string(_content.begin(), endWord));
}

void Rule::eraseWord(size_t size)
{
	_content.erase(0, size);
}

bool Rule::empty(void)
{
	return (_content.empty());
}

void Rule::set_content(std::string newContent)
{
	_content = newContent;
}

const std::string Rule::get_content(void)const
{
	return (_content);
}

std::string Rule::get_content(void)
{
	return (_content);
}