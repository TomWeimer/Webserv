#include "Input.hpp"


std::string 	createInputFromFile(std::string filename)
{
	std::ifstream	file;
	std::string		line;
	std::string		fileContent;

	file.open(filename.c_str());
	if (file.good() == false)
		throw (std::runtime_error("The config file could'nt be opened\n"));
	while (getline(file, line))
	{
		fileContent += line;
		if (file.peek() != EOF)
			fileContent += "\n";	
	}
	file.close();
	return(fileContent);
}

void Input::append(std::string word)
{
	_content.append(" ");
	_content.append(word);
}

void Input::fill(std::string filename)
{
	std::ifstream	file;
	std::string		line;
	std::string		fileContent;

	file.open(filename.c_str());
	if (file.good() == false)
		throw (std::runtime_error("The config file could'nt be opened\n"));
	while (getline(file, line))
	{
		fileContent += line;
	}
	file.close();
	_content = fileContent;
	
}

std::string Input::nextWord(void)
{
	std::string::iterator endWord;
	std::string::iterator endContent;

	endWord = _content.begin();
	endContent = _content.end();
	if (std::isspace(_content[0]) == true)
	{
		skip_and_erase_while_space(_content, endWord, endContent);
		//return (" ");
	}
	skip_until_space(endWord, endContent);
	return (std::string(_content.begin(), endWord));
}

void Input::eraseWord(size_t size)
{
	std::cerr << "size: " << size << std::endl;
	_content.erase(0, size);
}

bool Input::empty(void)
{
	return (_content.empty());
}

void Input::set_content(std::string newContent)
{
	_content = newContent;
}

const std::string Input::get_content(void)const
{
	return (_content);
}

std::string Input::get_content(void)
{
	return (_content);
}