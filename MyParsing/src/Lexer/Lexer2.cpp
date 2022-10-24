#include "Lexer2.hpp"
#include <cstdlib>

std::ostream& operator<<(std::ostream& out, const KeyWord& origin)
{
	std::vector<Token>::const_iterator it;
	out << "KeyWord: " << BOLDWHITE << origin.tokenType << RESET << std::endl;

	it = origin.args.begin();
	while (it != origin.args.end())
	{
		out << "               - " << BOLDCYAN << it->tokenType << " " << RESET << it->valueToken << std::endl;
		it++;
	}
	return (out);
}

std::ostream& operator<<(std::ostream& out, const Token& origin)
{
	out << "Token: " << origin.tokenType << std::endl;
	out << "Token Value: " << origin.valueToken << std::endl;
	return (out);
}

void Lexer::set_input(std::string input) {
	_input = input;
}

std::vector<KeyWord> Lexer::lexeme()
{
	std::string line;
	std::string ruleName;
	std::string ruleContent;

	line = _input.next_line();
	while (line.empty() == false)
	{
	for (size_t ruleIndex = 0; ruleIndex < _rules.size() && line.empty() == false; ruleIndex++)
	{
		ruleName = _rules[ruleIndex];
		ruleContent = _rules[ruleName];
		if (match_rule(line, ruleContent, ruleName) == true)
			break ;
	}
	line = _input.next_line();
	}
	return (_list);
}


bool Lexer::match_rule(std::string& line, std::string& rule, std::string tokenName)
{
	Input	inputLine;
	Rule	actualRule;
	bool	result;
	KeyWord	newToken;
//	std::vector<std::string> match_reserve;
//	std::vector<std::string> match_value;

	inputLine.set_content(line);
	actualRule.set_content(rule);
	#ifdef DEBUG
	std::cerr << "\nRULE:	" << BOLDYELLOW << tokenName << RESET << std::endl;
	std::cerr << "––––––––––––––––––––––––––––––––––––––––––––––––" << std::endl;
	#endif
	newToken.tokenType = tokenName;
	while (inputLine.empty() == false && actualRule.empty() == false)
	{
		copyInputLine = inputLine.get_content();
		_actualToken = tokenName;
		result = match_word(inputLine, actualRule);
		if (_actualToken != tokenName)
		{
			_tokenValue = copyInputLine.substr(0, copyInputLine.size() - inputLine.get_content().size());
			// std::cerr << "->" << tokenName << "<-" << std::endl;
			// std::cerr << "->" << new_keyWord(_tokenValue.begin(), _tokenValue.end()) << "<-" << std::endl;
			newToken.args.insert(newToken.args.end(), Token( _actualToken , new_keyWord(_tokenValue.begin(), _tokenValue.end())));
			
		}
		_tokenValue.clear();
		if (result == false)
			break;
	}
	if (result == true && (inputLine.empty() == false || actualRule.empty() == false))
	{
		result = false;
	#ifdef DEBUG
		if (actualRule.empty() == false)
			std::cerr << "rule not empty: ->" << actualRule.get_content() << "<-" << std::endl; 
		if (inputLine.empty() == false)
			std::cerr << "input not empty: ->" << inputLine.get_content() << "<-" << std::endl; 
	#endif
	}
	if (result == true)
	{
		_list.insert(_list.end(), newToken);		
	}
	#ifdef DEBUG
	std::cerr << "––––––––––––––––––––––––––––––––––––––––––––––––" << std::endl;
	result ? std::cerr <<  BOLDGREEN << "rule match " << BOLDYELLOW << tokenName : std::cerr << BOLDRED << "rule not match  " << BOLDYELLOW << tokenName;
	std::cerr << RESET << std::endl;
	#endif
	//exit(1);
	return (result);
}


bool Lexer::match_rule(Input& inputLine, Rule& actualRule)
{
	bool	result;

	while (inputLine.empty() == false && actualRule.empty() == false)
	{
		result = match_word(inputLine, actualRule);
		if (result == false)
			break;
	
	}

	return (result);
}




bool Lexer::match_subrule(Input& inputLine, Rule& actualRule)
{
	bool result;

	while (inputLine.empty() == false && actualRule.empty() == false)
	{
		result = match_word(inputLine, actualRule);

	}
	return (result);
}


bool Lexer::match_word(Input& inputLine, Rule& actualRule)
{
	
	Operator	op;
	bool		result;

	op = actualRule.next_operator();
	result = false;
	if (op > 0)
	{
		if (op == SECONDQUOTE)
			result = match_string(inputLine, actualRule);
		else if (op == CONCATENATION)
			result = match_concatenation(inputLine, actualRule);
		else if (op == GROUPSTART)
			result = match_group(inputLine, actualRule);
		else if (op == REPEATSTART)
			result = match_repeat(inputLine, actualRule);
		else if (op == CHOICE)
		{
			result = match_choice(inputLine, actualRule);
		}
		else if (op == SPECIAL)
			result = match_special(inputLine, actualRule);
		else if (op == TOKENSTART)
		{
			result = match_token(inputLine, actualRule);	
		}
	}
	return (result);
}

bool Lexer::match_token(Input& inputLine, Rule& actualRule)
{
	Input		newInput;
	Rule		tokenRule;
	std::string tokenName;
	bool		result;

	tokenName = actualRule.nextWord();
	actualRule.advance(tokenName.size());
	actualRule.clear_until_pos();

	#ifdef DEBUG
	std::cerr << "\nTOKEN: " << BLUE << tokenName << RESET << std::endl;
	#endif
	tokenRule.set_content(_rules[tokenName]);
	newInput.set_content(inputLine.restLine());
	// std::string restline = inputLine.restLine();
	// std::string rule = _rules[tokenName];
	if (match_rule(newInput, tokenRule) == true)
	{
		_actualToken = tokenName;
		size_t size;
		size =  inputLine.get_content().size() - newInput.get_content().size();
		inputLine.move_pos(size);
		inputLine.clear_until_pos();
		result = true;
	}
	else
		result = false;
	#ifdef DEBUG
	std::cerr << "match_token: <" << BLUE << tokenName << RESET << "> ";
	result ? std::cerr << GREEN << "	match"  : std::cerr <<  RED << "	not match";
	std::cerr << RESET << std::endl;
	#endif
	return (result);

}

bool Lexer::match_string(Input& inputLine, Rule& actualRule)
{
	
	std::string ruleString;
	std::string inputString;
	bool		result;

	ruleString = actualRule.get_next_string();
	inputString = inputLine.nextWord();

	#ifdef DEBUG
	std::cerr << "match_string: ";
	#endif
	
	if (strncmp(ruleString.c_str(), inputString.c_str() , ruleString.size()) == 0)
	{
		inputLine.advance(ruleString.size());
		inputLine.clear_until_pos();
		result = true;
	}
	else
		result = false;
	actualRule.advance(ruleString.size() + 2);
	actualRule.clear_until_pos();
	#ifdef DEBUG
	result ? std::cerr << GREEN << "		match"  : std::cerr << RED << "		not match";
	std::cerr << RESET << std::endl;
	#endif
	return (result);
}

bool Lexer::match_concatenation(Input& inputLine, Rule& actualRule)
{
	bool result;
	std::string lhs;
	std::string rhs;

	lhs = actualRule.nextWord();
	actualRule.advance(lhs.size());
	actualRule.clear_until_pos();

	if (lhs == ",")
	{
		rhs = actualRule.nextWord();
		actualRule.advance(rhs.size());
		actualRule.clear_until_pos();
		result = make_concatenation(inputLine, "", rhs);
	}
	else
	{
		actualRule.advance(actualRule.nextWord().size());
		actualRule.clear_until_pos();
		rhs = actualRule.nextWord();
		actualRule.advance(rhs.size());
		actualRule.clear_until_pos();
		result = make_concatenation(inputLine, lhs, rhs);
	}
	
	return (result);
}

bool Lexer::make_concatenation(Input& inputLine, std::string lhs, std::string rhs)
{
	Rule		newRule;
	bool		result;

	#ifdef DEBUG
	std::cerr << "match_concatenation:		" << std::endl;
	#endif
	if (lhs.empty() == false)
	{
		newRule.set_content(lhs);
		result = match_word(inputLine, newRule);
	}
	else
		result = true;
	if (result == true && std::isspace(inputLine.peek()) == false)
	{
		newRule.set_content(rhs);
		result = match_word(inputLine, newRule);
	}
	else
		result = false;
	return (result);
}

bool Lexer::make_choice(Input& inputLine, std::string lhs, std::string rhs)
{
	Rule		newRule;
	bool		result;

	#ifdef DEBUG
	std::cerr << "match_choice:		" << std::endl;
	#endif
	newRule.set_content(lhs);
	result = match_word(inputLine, newRule);
	if (result != true)
	{
		newRule.set_content(rhs);
		result = match_word(inputLine, newRule);
	}
	return (result);
}

bool Lexer::match_group(Input& inputLine, Rule& actualRule)
{
	Operator	op;
	std::string::iterator pos;
	std::string lhs;
	std::string rhs;
	bool		result;
	bool		stop;

	#ifdef DEBUG
	std::cerr << "start group: \n" << std::endl;
	#endif
	std::string groupContent = actualRule.get_next_group_sequence();
	stop = false;
	result = false;

	pos = groupContent.begin() + 1;
	actualRule.advance(1);
	actualRule.clear_until_pos();
	while (stop == false)
	{
		if (lhs.empty() == true)
			lhs = actualRule.nextWord();
		else
			lhs = rhs;
		actualRule.advance(lhs.size());
		actualRule.clear_until_pos();
		
		op = actualRule.next_operator();
		actualRule.advance(actualRule.nextWord().size());
		actualRule.clear_until_pos();
		rhs = actualRule.nextWord();
		if (op == GROUPEND)
			stop = true;
		else
		{
			
			if (op == CHOICE)
			{
				result = make_choice(inputLine, lhs, rhs);
			}
			else if (op == SECONDQUOTE)
			{
				Rule newRule;
				newRule.set_content(lhs);
				result = match_string(inputLine, newRule);
				if (result == true)
				{
					inputLine.advance(lhs.size() - 2);
					inputLine.clear_until_pos();
		
				}
				else
				{
					newRule.set_content(rhs);
					result = match_string(inputLine, newRule);
					if (result == true)
					{
						inputLine.advance(rhs.size() - 2);
						inputLine.clear_until_pos();
						stop = true;
					}
				}
			
			}
			stop = result;
		}
		}
		if (result == true)
		{
			size_t pos_last;
			actualRule.clear_until_pos();
			pos_last = actualRule.operator_pos(GROUPEND);
			actualRule.eraseWord(pos_last + 1);
		}
	#ifdef DEBUG
	std::cerr << "\nend group: ";
	result ? std::cerr << GREEN << " match"  : std::cerr << RED << " not match";
	std::cerr << RESET << std::endl;
	#endif
	return (result);
}


bool Lexer::match_choice(Input& inputLine, Rule& actualRule)
{
	std::string lhs;
	std::string rhs;
	bool		result;

	lhs = actualRule.nextWord();
	actualRule.advance(lhs.size());
	actualRule.clear_until_pos();

	actualRule.advance(actualRule.nextWord().size());
	actualRule.clear_until_pos();

	rhs = actualRule.nextWord();
	actualRule.advance(rhs.size());
	actualRule.clear_until_pos();
	result = make_choice(inputLine, lhs, rhs);
	return (result);
}

bool Lexer::match_repeat(Input& inputLine, Rule& actualRule)
{
	bool result;
	bool result_word;

	Rule newRule;
	std::string ruleContent;
	ruleContent = actualRule.get_next_sequence(REPEATSTART);
	#ifdef DEBUG
	std::cerr << "\nstart repeat: " << "{ " << BLUE <<  ruleContent << RESET << " }" << std::endl;
	#endif
		size_t pos_op = actualRule.operator_pos(REPEATSTART);
		newRule.set_content(actualRule.left_hand_side(pos_op));
		if (newRule.empty() == false)
		{
			if (match_word(inputLine, newRule) == false)
			{
				return (false);
			}
		}
		actualRule.eraseWord(pos_op);
		actualRule.clear_until_pos();
	newRule.set_content(ruleContent);
	
	result = match_word(inputLine, newRule);
	result_word = result;
	while (inputLine.empty() == false && result_word == true)
	{
		//std::cerr << "HELLO2" << std::endl;
		newRule.set_content(ruleContent); 
		result_word = match_word(inputLine, newRule); // <-
	}
	actualRule.advance(ruleContent.size());
	actualRule.clear_until_pos();

	#ifdef DEBUG
	std::cerr << "\nend repeat: ";
	result ? std::cerr << GREEN << " match"  : std::cerr << RED << " not match";
	std::cerr << RESET << std::endl;
	#endif
	
	return (result);
}

bool Lexer::match_special(Input& inputLine, Rule& actualRule)
{
	//(void)actualRule;
	static bool start_reading = false;
	static bool end_reading = false;
	bool result;
	std::string buffer;
	char		inputChar;

	buffer = actualRule.nextWord();
	inputChar = inputLine.peek();
	if (inputLine.empty() == false)
	{
		if (start_reading == false && end_reading == false)
		{
			if (std::isspace(inputChar) == true)
			{
				while (std::isspace(inputChar) == true)
				{
					inputLine.move_pos(1);
					inputLine.clear_until_pos();
					inputChar = inputLine.peek();
				}
			}
			start_reading = true;
		}
		else if (start_reading == true && end_reading == false && std::isspace(inputChar) == true)
		{
			end_reading = true;
			result = false;
		}
		if (inputLine.empty() == false && start_reading == true && end_reading == false)
		{
			inputLine.move_pos(1);
			inputLine.clear_until_pos();
			result = true;
		}
	}
	if (inputLine.empty() == true)
		end_reading = true;
	if (end_reading == true )
	{
		start_reading = false;
		end_reading = false;
	}
	actualRule.eraseWord(buffer.size() + 2);
	return (result);
}

Lexer::Lexer(std::string grammarFileName, std::string inputFileName)
	: _rules(Vocabulary(grammarFileName)), _input(inputFileName) {}

Lexer::Lexer(const Vocabulary &vocabulary, const Input &otherInput)
	: _rules(vocabulary), _input(otherInput) {}

Lexer::Lexer(const Vocabulary &vocabulary, std::string InputContent)
	: _rules(vocabulary), _input()
{
	_input.set_content(InputContent);
}

Lexer::Lexer(const Lexer &origin)
	: _rules(origin.get_rules()), _input(origin.get_input()) {}

Lexer &Lexer::operator=(const Lexer &origin)
{
	if (this == &origin)
		return (*this);
	_rules = origin.get_rules();
	_list = origin.get_list();
	_input = origin.get_input();
	return (*this);
}

Lexer::~Lexer() {}

Input 				&Lexer::get_input() { return (_input); }
const Input			&Lexer::get_input() const { return (_input); }

Vocabulary			&Lexer::get_rules() { return (_rules); }
const Vocabulary	&Lexer::get_rules() const { return (_rules); }

std::vector<KeyWord>		Lexer::get_list() { return (_list); }
const std::vector<KeyWord>	Lexer::get_list() const { return (_list); }
