#include "utils/Parsing/Input.hpp"

Input::Input()
	: _size_check(true) {}

Input::Input(std::string filename)
	: _size_check(true)
{
	std::string content;

	content = content_from_file(filename);
	formatInput(content);
}

std::string Input::content_from_file(std::string filename)
{

	std::ifstream file;
	std::string fileContent;

	open_file(file, filename);
	read_file(file, fileContent);
	close_file(file);
	return (fileContent);
}

void Input::open_file(std::ifstream &file, std::string filename)
{
	file.open(filename.c_str());
	if (file.good() == false)
		throw (InputException("The config file couldn't be opened\n"));
}

void Input::read_file(std::ifstream &file, std::string &fileContent)
{
	fileContent.assign(ifstream_it(file), ifstream_it());
	if (fileContent.empty() == true)
		throw (InputException("The config file was empty!\n"));
}

void Input::close_file(std::ifstream &file)
{
	file.close();
}

void Input::formatInput(std::string &content)
{
	_word_nb = 0;
	_line_nb = 0;
	_wordList.clear();
	_lineList.clear();
	erase_semicolon(content);
	format_newline(content);
	split_content(content);
	erase_extra_whitespace();
}

void Input::erase_semicolon(std::string &content)
{
	size_t pos;

	pos = content.find(';');
	while (pos != std::string::npos)
	{
		content.erase(pos, 1);
		pos = content.find(';');
	}
}

void Input::format_newline(std::string &content)
{
	size_t pos;

	pos = content.find("\r\n");
	while (pos != std::string::npos)
	{
		content.erase(pos, 1);
		pos = content.find("\r\n");
	}
}

void Input::split_content(std::string &content)
{
	std::string line;
	std::stringstream inputContent(content);

	while (std::getline(inputContent, line))
	{
		if (is_empty_line(line) == false)
			_lineList.push_back(line);
	}
}

void Input::erase_extra_whitespace()
{
	std::string tmp;
	std::string actualLine;
	std::stringstream lineStream;

	for (size_t index = 0; index < _lineList.size(); index++)
	{
		actualLine = _lineList[index];
		lineStream.clear();
		for (size_t i = 0; std::isspace(actualLine[i]) == true; i++)
		{
			actualLine.erase(i, 1);
		}
		for (size_t i = actualLine.size() - 1; std::isspace(actualLine[i]) == true; i--)
		{
			actualLine.erase(i, 1);
		}
	//	std::cerr << "actualine: ->" << actualLine << "<-" << std::endl;
		lineStream << actualLine;
	
		//std::cerr << "actualine: " << actualLine << std
		actualLine.clear();
		//std::cerr << "actualine: " << actualLine << std::endl;
		for (; lineStream.eof() == false;)
		{
			lineStream >> tmp;
			actualLine += tmp;
			if (lineStream.eof() == false)
				actualLine += " ";
		}
		lineStream.clear();
		tmp.clear();
		//std::cerr << "actualine: ->" << actualLine << "<-" << std::endl;
		_lineList[index].swap(actualLine);
		
		actualLine.clear();
		lineStream.clear();
	}
}

bool Input::is_empty_line(std::string& line)
{
	for (std::string::iterator it = line.begin(); it != line.end() ; it++)
	{
		if (std::isspace(*it) == false)
			return (false);
	}
	return (true);
}


std::string Input::next_line()
{
	std::string line;

	if (_line_nb == _lineList.size())
	{
		_wordList.clear();
		return ("");
	}
	line = _lineList[_line_nb];
	_line_nb++;
	split_line(line);
	_word_pos = _wordList[0].begin();
	_word_end = _wordList[0].end();
	_size_check = true;
	return (line);
}

std::string Input::nextWord(std::string::iterator pos, std::string::iterator last)
{
	std::string word;

	if (pos >= last)
		return ("");
	for (;pos != last && std::isspace(*pos) == true; pos++)
	{}
	for (;pos != last && std::isspace(*pos) == false; pos++)
	{
		word += *pos;
	}
	return (word);
}

void Input::split_line(std::string line)
{
	std::string word;
	std::stringstream streamline(line);

	_wordList.clear();
	while (std::getline(streamline, word, ' '))
	{
		_wordList.push_back(word);
	}

}


std::string Input::operator[](size_t index)
{
	if (index >= _lineList.size())
		return ("");
	return (_lineList[index]);
}

size_t	Input::compare_value(std::string value, std::string::iterator pos, std::string::iterator last)
{
	std::string word;
	word = nextWord(pos, last);
	//std::cerr << "word: ->" << word  << "<- value: ->" << value  << "<-" << std::endl;
	//std::cerr << value.size() << std::endl;
	if (strncmp(word.c_str(), value.c_str(), value.size()) == 0)
	{
	//	std::cerr << "newWord: " << word << std::endl;
		if (_size_check == true && word.size() != value.size())
			return(0);
		return (value.size());
	}
	return (0);
}
void Input::set_content(std::string newContent)
{
	formatInput(newContent);
}
Input::Input(const Input &otherInput)
	: _lineList(otherInput._lineList), _wordList(otherInput._wordList),
	  _line_nb(otherInput._line_nb), _word_nb(otherInput._word_nb)
{
}

void Input::size_check_on(bool on)
{
	_size_check = on;
}

bool Input::get_size_check()
{
	return (_size_check);
}

Input &Input::operator=(const Input &otherInput)
{
	if (this == &otherInput)
		return (*this);
	_lineList = otherInput._lineList;
	_wordList = otherInput._wordList;
	_line_nb = otherInput._line_nb;
	_word_nb = otherInput._word_nb;
	return (*this);
}

Input::~Input() {}