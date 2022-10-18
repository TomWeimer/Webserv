#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include "Input.hpp"
#include "Rule.hpp"
#include "Vocabulary.hpp"
#include "Operator.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

class Lexer
{
private:
	typedef bool (Lexer::*match_t)(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord);
	std::map<Operator, match_t>			_match;
	std::map<Operator, std::string>		_operator_name;
	Vocabulary						_rules;

public:
	Lexer(Vocabulary& tokensRules);

public:
	std::vector<std::string> lexeme(std::string configFile);
	std::string recognize(std::string line);
	bool match_rule(Input& input, Rule& rule);

private:
	bool match_word(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord);
	bool match_operator(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord, Operator type);
	bool match_token(Input& input, Rule& rule, std::string& inputWord, std::string& tokenName);

	// operator ""
	bool match_string(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord);
	// operator ,
	bool match_addition(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord);

};

#endif