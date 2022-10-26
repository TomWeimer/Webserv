#ifndef LEXER_HPP
#define LEXER_HPP
// #define DEBUG 1
#include <vector>
#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include "Input.hpp"
#include "Rule.hpp"
#include "Vocabulary.hpp"
#include "Operator.hpp"

#define RESET "\033[0m"
#define BLACK "\033[30m"			  /* Black */
#define RED "\033[31m"				  /* Red */
#define GREEN "\033[32m"			  /* Green */
#define YELLOW "\033[33m"			  /* Yellow */
#define BLUE "\033[34m"				  /* Blue */
#define MAGENTA "\033[35m"			  /* Magenta */
#define CYAN "\033[36m"				  /* Cyan */
#define WHITE "\033[37m"			  /* White */
#define BOLDBLACK "\033[1m\033[30m"	  /* Bold Black */
#define BOLDRED "\033[1m\033[31m"	  /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"	  /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"	  /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"	  /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"	  /* Bold White */
#define LHS 0
#define RHS 1


struct Token
{
	std::string tokenType;
	std::string valueToken;
	Token() {}
	Token(std::string name, std::string value)
		: tokenType(name), valueToken(value) {}
	
};

struct KeyWord
{
	std::string tokenType;
	std::vector<Token> args;

};

std::ostream& operator<<(std::ostream& out, const KeyWord& origin);


std::ostream& operator<<(std::ostream& out, const Token& origin);

class Lexer
{
private:
	Vocabulary				_rules;
	Input					_input;
	std::string copyInputLine;

	std::vector<KeyWord>	_list;
	
	std::string::iterator	_first;
	std::string::iterator	_last;
	std::string				_actualToken;
	std::string	_tokenValue;

public:
	std::vector<std::string>	_value;
	Vocabulary& get_rules();
	Input&		get_input();
	std::vector<KeyWord>	get_list();
	const Vocabulary&		get_rules()const;
	const Input&			get_input()const;
	const std::vector<KeyWord> get_list()const;

private:
	Lexer();

public:
// Constructor
	Lexer(const Vocabulary &vocabulary, const Input& InputContent);
	Lexer(const Vocabulary &vocabulary, std::string InputContent);
	Lexer(std::string grammarFileName, std::string inputFileName);
// Copy Constructor
	Lexer(const Lexer &origin);
// Assignement
	Lexer &operator=(const Lexer &origin);
// Destructor
	~Lexer();

public:
	void				 set_input(std::string input);
	std::vector<KeyWord> lexeme();
private:
	bool match_rule(std::string& line, std::string& rule, std::string tokenName);
	bool match_word(Input& inputLine, Rule& actualRule);

	bool match_token(Input& inputLine, Rule& actualRule);
	bool match_string(Input& inputLine, Rule& actualRule);
	bool match_concatenation(Input& inputLine, Rule& actualRule);
	bool match_group(Input& inputLine, Rule& actualRule);
	bool make_concatenation(Input& inputLine, std::string lhs, std::string rhs);
	bool match_choice(Input& inputLine, Rule& actualRule);
	bool make_choice(Input& inputLine, std::string lhs, std::string rhs);
	bool match_repeat(Input& inputLine, Rule& actualRule);
	bool match_special(Input& inputLine, Rule& actualRule);

	bool match_subrule(Input& line, Rule& rule);
	bool match_rule(Input& inputLine, Rule& actualRule);
};

std::ostream& operator<<(std::ostream& out, const std::vector<KeyWord>& tokenList);

#endif