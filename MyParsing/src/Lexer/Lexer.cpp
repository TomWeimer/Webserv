#include "Lexer.hpp"
#include <cstring>
#include "utils.hpp"
#include <string>
#include "Input.hpp"


std::vector<std::string> Lexer::lexeme(std::string configFile)
{
	std::vector<std::string>		v;
	std::string					fileContent;
	std::string					line;
	fileContent  = createInputFromFile(configFile);

	std::cerr << fileContent << std::endl;
	for (size_t j = 0; j < fileContent.size(); j++)
	{
		
		if (fileContent[j] == '\n' || j == fileContent.size() - 1)
		{
			line += fileContent[j];
			if (only_whitespace(line.begin(), line.end()) == false)
				v.push_back(recognize(line));
			std::cerr << "OK" << std::endl;
			line.clear();
		}
		else
			line += fileContent[j];
	}
	return (v);
}

std::string Lexer::recognize(std::string line)
{
	Vocabulary::iterator first;
	Vocabulary::iterator last;
	std::string	nameToken;
	Rule		ruleToken;
	Input		copyInput;

	std::cerr << "size before" << line.size() << std::endl;
	if (line.empty() == false && line[line.size() - 1] == '\n')
		line.erase(line.size() - 1, 1);
	std::cerr << "size after" << line.size() << std::endl;
	std::cerr << "LINE: "<< line << std::endl;

	if (_rules.empty() == false)
	{
		first = _rules.begin();
		last = _rules.end();
		
		for (; first != last; first++)
		{
			nameToken = _rules[first->first];
			std::cerr << nameToken << std::endl;
			ruleToken.fill(_rules[nameToken]);
			copyInput.set_content(line);
			
			if (match_rule(copyInput, ruleToken) == true)
			{
					std::cerr << BOLDGREEN << "\nRULE MATCH" << RESET << std::endl;
					std::cerr << "------------------------------------------------------------------" << std::endl;
				return (nameToken);
			}
			else
			{
					std::cerr << BOLDRED << "\nRULE NOT MATCH" << RESET << std::endl;
					std::cerr << "------------------------------------------------------------------" << std::endl;
			}
			

		}
	}
	return ("");
}

bool Lexer::match_rule(Input& input, Rule& rule)
{
	std::string inputWord;
	std::string ruleWord;

		std::cerr << "------------------------------------------------------------------" << std::endl;
		std::cerr << "RULE START:" << std::endl;
		std::cerr << "rule content:" << rule.get_content() << std::endl;
			std::cerr << "input: " << input.empty() << std::endl;
			std::cerr << "rule: " << rule.empty() << std::endl;
	while (input.empty() == false && rule.empty() == false)
	{

		inputWord = input.nextWord();
		ruleWord = rule.nextWord();
		rule.eraseWord(ruleWord.size());
		if (match_word(input, rule, inputWord, ruleWord) == false)
		{
			std::cerr << "ruleWord: " << ruleWord << std::endl;
			std::cerr << "result: " << BOLDRED << "FALSE" << RESET << std::endl;
			return (false);
		}
		std::cerr << "ruleWord: " << ruleWord << std::endl;
		std::cerr << "result: " << BOLDGREEN << "OK" << RESET << std::endl;
		std::cerr << "input: " << input.get_content() << std::endl;
		input.eraseWord(ruleWord.size());
		
		std::cerr << "input: " << input.get_content() << std::endl;
		std::cerr << "rule: " << rule.get_content() << std::endl;
	}
	if (input.empty() == false || rule.empty() == false)
	{
		return (false);
	}
		
	return (true);
}

bool Lexer::match_word(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord)
{
	Operator type;
	type = is_operator(ruleWord);
	if (type > 0)
		return (match_operator(input, rule, inputWord, ruleWord, type));
	return (match_token(input, rule, inputWord, ruleWord));
}

bool Lexer::match_operator(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord, Operator type)
{
	match_t function;
	bool	result;
	std::cerr << "TYPE: " << type << std::endl;
	std::cerr << BOLDWHITE << "\noperator		" << RESET
	<< BLUE << _operator_name[type]
	<< RESET << std::endl;

	std::cerr << "input:	" << input.get_content() << " 	inputWord: " << inputWord  << std::endl;
	std::cerr << "rule:	" << rule.get_content() << "	ruleWord: " << ruleWord  << std::endl;

	
	function = _match[type];
	result = (this->*function)(input, rule, inputWord, ruleWord);
	
	return (result);
}

bool Lexer::match_token(Input& input, Rule& rule, std::string& inputWord, std::string& tokenName)
{
	(void)rule;
	(void)input;
	Input		newInput;
	Rule		newRule;
	std::string	ruleContent;

	std::cerr << BOLDWHITE << "\ntoken		" << RESET
	<< BLUE << "<"
	<< WHITE << tokenName
	<< BLUE << "> "
	<< RESET  << std::endl;

	ruleContent = _rules[tokenName];

	std::cerr << "ruleContent: "  <<  "->"<< ruleContent << "<-" << std::endl;
	if (ruleContent.empty() == true)
		return (false);
	newRule.set_content(ruleContent);
	newInput.set_content(inputWord);
	if (match_rule(newInput, newRule) == false)
		return (false);
	return (true);
} 
// revoir

Lexer::Lexer(Vocabulary &tokenRules)
	: _rules(tokenRules)
{
	std::string operatorName;

	_match.insert(std::make_pair(SECONDQUOTE, 	&Lexer::match_string));
	operatorName.clear();
	operatorName = "\"...\"";
	
	_operator_name.insert(std::make_pair(SECONDQUOTE, operatorName));

	_match.insert(std::make_pair(CONCATENATION, &Lexer::match_addition));
	operatorName.clear();
	operatorName =  "... , ...";
	_operator_name.insert(std::make_pair(CONCATENATION, operatorName));
}
