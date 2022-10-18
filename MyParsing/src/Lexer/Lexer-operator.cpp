#include "Lexer.hpp"
#include <cstring>
#include "utils.hpp"

bool Lexer::match_string(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord)
{
	(void)input;
	(void)rule;
	(void)inputWord;
	(void)ruleWord;
	size_t firstQuote;
	size_t secondQuote;
	std::string match;


	// format input
	std::cerr << "1ALOONE" << std::endl;
	std::cerr << "\n2input:|" << input.get_content() << "|  rule: |" << rule.get_content() << "|" << std::endl;
	std::cerr << "3inputWord: |" << inputWord << "|   ruleWord: |" << ruleWord <<"|"<< std::endl;
	firstQuote = ruleWord.find_first_of('\"');
	ruleWord.erase(firstQuote, 1);
	secondQuote = ruleWord.find_first_of('\"');
	ruleWord.erase(secondQuote, 1);
	if (std::strncmp(input.get_content().c_str(), ruleWord.c_str(), ruleWord.size()) != 0)
		return (false);
	return (true);
}

bool Lexer::match_addition(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord)
{
	if (inputWord.empty() == true)
	{
		std::cerr << "EMPTY" << std::endl;
		inputWord = input.nextWord();
	}
	ruleWord = rule.nextWord();
	rule.eraseWord(ruleWord.size());
	std::cerr << "1ALOONE" << std::endl;
	std::cerr << "\n2input:|" << input.get_content() << "|  rule: |" << rule.get_content() << "|" << std::endl;
	std::cerr << "3inputWord: |" << inputWord << "|   ruleWord: |" << ruleWord <<"|"<< std::endl;
	// std::cerr << "inputWord: " << inputWord << " ruleWord: " << ruleWord << std::endl;
	return (match_word(input, rule, inputWord, ruleWord));
}


