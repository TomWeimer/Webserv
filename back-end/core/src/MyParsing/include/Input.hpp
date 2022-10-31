#ifndef INPUT_HPP
#define INPUT_HPP
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

class Input
{
	typedef std::istreambuf_iterator<char> ifstream_it;

private:
	std::vector<std::string> 	_lineList;
	std::vector<std::string> 	_wordList;
	size_t						_line_nb;
	size_t						_word_nb;
	std::string::iterator		_word_pos;
	std::string::iterator		_word_end;
	bool						_size_check;
	

public:
	Input();
	Input(std::string filename);
	Input(const Input &otherInput);
	Input &operator=(const Input &otherInput);
	~Input();

private:
	std::string content_from_file(std::string filename);
	void open_file(std::ifstream &file, std::string filename);
	void read_file(std::ifstream &file, std::string &fileContent);
	void close_file(std::ifstream &file);
	void formatInput(std::string &content);
	void format_newline(std::string &content);
	void split_content(std::string &content);
	void erase_extra_whitespace(void);
	void erase_empty_line(void);
	bool is_empty_line(std::string& line);
	void erase_semicolon(std::string &content);
	

public:
	std::string operator[](size_t index);

	// void	copy_to_token();
	//std::string Input::actualWord(void);
	std::string obtain_token_content(size_t start, size_t end);
	bool	match_all_words();
	void	advance(size_t size);
	bool	get_size_check(void)const;
	void  size_check_on(bool on);
	std::string next_line(void);
	std::string nextWord(void);
	std::string nextWord(std::string::iterator pos, std::string::iterator last);
	char		peek();
	bool		empty();
	void set_content(std::string newContent);
	size_t word_nb()const;
	size_t size_line()const;
	std::string get_line()const;
	std::string multiple_words(size_t start_str, size_t end_str);
	bool get_size_check();
	size_t compare_value(std::string value, std::string::iterator pos, std::string::iterator last);
	bool is_at_end_word();

private:
	void split_line(std::string line);
};

class InputException : public std::runtime_error
{
public:
	InputException(const char *what) : runtime_error(what) {}
};

#endif