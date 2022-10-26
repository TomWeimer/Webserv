#include "Input.hpp"

void Input::openFile(std::ifstream& file, std::string& filename)
{
	file.open(filename.c_str());
	if (file.good() == false)
		throw (std::runtime_error("The config file could'nt be opened\n"));
}

std::string Input::readFile(std::ifstream& file)
{
	std::string line;
	std::string fileContent;

	while (getline(file, line))
	{
		fileContent += line;
		if (file.peek() != EOF)
			fileContent += "\n";	
	}
	file.close();
	return (fileContent);
}

void Input::createInputFromFile(std::string filename)
{
	std::ifstream file;

	openFile(file, filename);
	_content = readFile(file);
}
Input::Input()
{
}

Input::Input(std::string filename)
{
	// std::cerr << "YOLOL" << std::endl;
	createInputFromFile(filename);
	_pos = _content.begin();
	_last = _content.end();
	// std::cerr <<  "p[1]: " << *_pos << std::endl;
}

Input::Input(const Input& otherInput)
	: _content(otherInput.get_content())
{
	_pos = _content.begin();
	_last = _content.end();
}

Input& Input::operator=(const Input& otherInput)
{
	if (this == &otherInput)
		return (*this);
	_content = otherInput.get_content();
	return (*this);
}
Input::~Input() {}

std::string Input::next_line()
{
	std::string::iterator	tmp;
	std::string				rtn;

	if (_pos == _last)
		return ("");
	tmp = _pos;
	skip_until(_pos, _last, '\n');
	rtn.insert(rtn.begin(), tmp, _pos);
	if (_pos != _last)
		_pos++;
	return (rtn);
}

std::string::iterator Input::begin() {
	return (_content.begin());
}

std::string::const_iterator Input::begin()const {
	return (_content.begin());
}

std::string::iterator Input::end() {
	return (_content.end());
}

std::string::const_iterator Input::end()const {
	return (_content.end());
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
	return (new_token(start, endWord));
}

void Input::eraseWord(size_t size)
{
	//std::cerr << "size: " << size << std::endl;
	_content.erase(0, size);
}

void Input::clear_until_pos()
{
	size_t distance;

	distance = _pos - _content.begin();

	if (_pos != _last)
	{
		_content.erase(0, distance);
	}
	else
		_content.clear();
	_pos = _content.begin();
	_last = _content.end();
}


bool Input::empty(void)
{
	return (_content.empty());
}

void Input::set_content(std::string newContent)
{
	_content = newContent;
	_pos = _content.begin();
	_last = _content.end();
}

const std::string Input::get_content(void)const
{
	return (_content);
}

std::string Input::get_content(void)
{
	return (_content);
}

std::string::const_iterator Input::get_pos(void)const
{
	return (_pos);
}

std::string::iterator Input::get_pos(void)
{
	return (_pos);
}

std::string Input::restLine()
{
	// std::cerr << "p[]: |" <<  *(_pos - 1) << "|" << std::endl;
	// std::cerr <<  *_content.begin() << std::endl;
	return (std::string(_pos, _last));
}

void Input::advance(size_t nb)
{
	while (nb > 0)
	{
		if (std::isspace(*_pos) == false)
			nb--;
		_pos++;
	}
}

void Input::move_pos(size_t nb)
{
	while (nb > 0)
	{
		nb--;
		_pos++;
	}
}

char Input::peek()
{
	return (*(_pos));
}

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