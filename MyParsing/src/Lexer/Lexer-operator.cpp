#include "Lexer.hpp"
#include <cstring>
#include "utils.hpp"
#include <cstdlib>
bool Lexer::compare_content(Input& input, std::string& ruleWord)
{
	std::cerr << "input: ->"  << input.get_content() << "<-" << std::endl;
	std::cerr << "rule: ->"  << ruleWord << "<-" << std::endl;
if (std::strncmp(input.get_content().c_str(), ruleWord.c_str(), ruleWord.size()) != 0)
{
	std::cerr << RED << "\nnot match content" << RESET << std::endl;
	return (false);
}
	std::cerr << GREEN << "\nmatch content" << RESET << std::endl;		
	return (true);
}

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
	firstQuote = ruleWord.find_first_of('\"');
	ruleWord.erase(firstQuote, 1);
	secondQuote = ruleWord.find_first_of('\"');
	ruleWord.erase(secondQuote, 1);
	return (compare_content(input, ruleWord));
	
}

bool Lexer::match_addition(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord)
{
	if (inputWord.empty() == true)
	{
		inputWord = input.nextWord();
	}
	ruleWord = rule.nextWord();
	rule.eraseWord(ruleWord.size());
	
	return (match_word(input, rule, inputWord, ruleWord));
}

bool Lexer::match_choice(Input& input, Rule& rule, std::string& inputWord, std::string& word)
{
	(void)rule;
	(void)inputWord;
	return (match_word(input, rule, inputWord, word));
}

bool Lexer::match_group(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord)
{
	(void)inputWord;
		size_t end = ruleWord.find_first_of(')');

		bool result;
		Operator type;
		std::string groupContent;
		std::string lhs;
		std::string rhs;
		std::string::iterator pos;

		groupContent.clear();
		groupContent.insert(0, rule.get_content(), 0, end);
		result = false;
		bool stop = false;
		pos = groupContent.begin();
		while (stop == false)
		{
			if (lhs.empty() == true)
				lhs = find_next_token_between_operator(pos, groupContent.end());
			else
				lhs = rhs;
			type = find_next_operator(pos, groupContent.end());
			if (type != GROUPEND)
			{
				//std::cerr << *pos	<< std::endl;
				pos++;			
				rhs = find_next_token_between_operator(pos, groupContent.end());
				
				
				if (type == CHOICE)
				{
					std::cerr << "CHOICE IN GROUP" << std::endl;
					if ((this->*_match[CHOICE])(input, rule, inputWord, lhs) == true)
					{
						result = true;
						stop = true;
						rule.eraseWord(groupContent.size());
						//input.eraseWord(input.get_content().size());
						ruleWord = lhs;
					}
					else if ((this->*_match[CHOICE])(input, rule, inputWord, rhs) == true)
					{
						result = true;
						stop = true;
						rule.eraseWord(groupContent.size());
						ruleWord = rhs;
					}
				}
			}
			else
			{
				stop = true;
			}
			
		}
	return (result);
}



bool Lexer::match_repeat(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord)
{
	(void)inputWord;
	(void)input;
	//(void)inputWord;
		size_t end = ruleWord.find_first_of('}');

		//bool result = false;
		bool return_value = false;
		//bool stop = false;
		// Operator type;
		std::string groupContent;
		// std::string lhs;
		// std::string rhs;
		std::string::iterator pos;

	

		groupContent.clear();
		groupContent.insert(0, rule.get_content(), 0, end);
		pos = groupContent.begin();
		std::cerr << "groupContent: " << groupContent << std::endl;
		std::string token = find_next_token_between_operator(pos, groupContent.end());
		std::string tokenCopy = token;
		Rule newRule;
		Input newInput;
		
		
		while (input.empty() == false)
		{
			newRule.set_content(_rules[token]);
			newInput.set_content(input.get_content());
			std::cerr << "INPUT BEFORE: " << input.get_content() << std::endl;
			std::cerr << "newRule: " << newRule.get_content() << std::endl;
			if (match_subrule(newInput, newRule) == false)
			{
				break;
			}
			else
				return_value = true;

			input.set_content(newInput.get_content());
		}
		if (return_value == true)
		{
			rule.eraseWord(groupContent.size());
			ruleWord = "";
		}
		std::cerr << "INPUT AFTER: " << input.get_content() << std::endl;
		//std::cerr << "repeat: " << ruleWord << std::endl;
	
		// result = false;
		// bool stop = false;
		// pos = groupContent.begin();
		// while (stop == false)
		// {
		// 	if (lhs.empty() == true)
		// 		lhs = find_next_token_between_operator(pos, groupContent.end());
		// 	else
		// 		lhs = rhs;
		// 	type = find_next_operator(pos, groupContent.end());
		// 	if (type != GROUPEND)
		// 	{
		// 		//std::cerr << *pos	<< std::endl;
		// 		pos++;			
		// 		rhs = find_next_token_between_operator(pos, groupContent.end());
				
				
		// 		if (type == CHOICE)
		// 		{
		// 			if ((this->*_match[CHOICE])(input, rule, inputWord, lhs) == true)
		// 			{
		// 				result = true;
		// 				stop = true;
		// 				rule.eraseWord(groupContent.size());
		// 				//input.eraseWord(input.get_content().size());
		// 				ruleWord = lhs;
		// 			}
		// 			else if ((this->*_match[CHOICE])(input, rule, inputWord, rhs) == true)
		// 			{
		// 				result = true;
		// 				stop = true;
		// 				rule.eraseWord(groupContent.size());
		// 				ruleWord = rhs;
		// 			}
		// 		}
		// 	}
		// 	else
		// 	{
		// 		stop = true;
		// 	}
			
		// }
		std::cerr << "RETURN VALUE " << return_value << std::endl;
	return (return_value);
}




bool Lexer::match_special(Input& input, Rule& rule, std::string& inputWord, std::string& ruleWord)
{
	(void)inputWord;
	if (input.empty() == false)
	{
		input.eraseWord(1);
		ruleWord = "";
		rule.eraseWord(rule.get_content().size());
		return (true);
	}
	return (false);
}