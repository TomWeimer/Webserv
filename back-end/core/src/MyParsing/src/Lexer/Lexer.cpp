#include "Lexer.hpp"
#include <cstdlib>

std::vector<KeyWord> Lexer::lexeme()
{
	std::string	name;
	
	_line = _input[0];
	for (size_t i = 1; _line.empty() == false; i++)
	{
		for (size_t j = 0; j < _rules.size(); j++)
		{
			name = _rules[j];
			_pos = _line.begin();
			if (match_rule(name, _rules[name]) == true)
			{
				_list.push_back(_actual_token);
				break ;
			}
		}
		_line = _input[i];
	}
	return (_list);
}

bool Lexer::match_rule(std::string ruleName, std::string ruleContent)
{
	Rule	rule(ruleContent, ruleName);
	bool	result;

	//rule.printBT();
	result = naviguate_in_tree(rule.get_root());
	if (ruleName.empty() == false && result == true)
	{
		if (_input.get_size_check() == true && _pos != _line.end())
			result = false;
	}	
	if (result == true )
		return (new_keyword(rule));
	return (false);
}

bool Lexer::new_keyword(Rule& rule)
{
	token *root;
	root = rule.get_root();
	_actual_token.clear();
	_actual_token.tokenType = rule.name();
	new_token(root);
	_list_tmp.push_back(_actual_token);
	return (true);
}


void Lexer::new_token(token *node)
{
	

	if (node == NULL)
		return ;
	if (node->type == TOKEN )
	{
		if (node->input_value.empty() == false)
			_actual_token.args.push_back(Token(node->value, node->input_value));
	}
	if (node->type == REPEAT && node->input_value.empty() == false)
	{
		std::stringstream split(node->input_value);
		std::string tmp;

		while (getline(split, tmp, ' '))
		{
			if (tmp.empty() == false)
				_actual_token.args.push_back(Token(node->value, tmp));	
		}
	}
	if (node->left != NULL)
		new_token(node->left);
	if (node->right != NULL)
		new_token(node->right);
}

bool Lexer::naviguate_in_tree(token *node)
{
	bool result;

	if (node == NULL)
		return (false);
	result = (this->*match_content[node->type])(node);
	if (node->type != CONCATENATION && node->type != CHOICE)
	{
		if (node->left != NULL && result == true)
		{
			result = naviguate_in_tree(node->left);
		}
		if (node->right != NULL && result == true)
		{
			result = naviguate_in_tree(node->right);
		}
	}
	return (result);
}

void Lexer::advance_pos(size_t size)
{
	while (_pos != _line.end() && size > 0)
	{
		_pos++;
		size--;
	}
}

std::string::iterator Lexer::start_pos()
{
	while (_pos != _line.end() && std::isspace(*_pos) == true)
	{
		_pos++;
	}
	return (_pos);
}

bool Lexer::compare_string(token *node)
{
	size_t size;
	std::string::iterator start;
	std::string::iterator last;

	start = start_pos();
	last = _line.end();
	size = _input.compare_value(node->value, _pos, last);
	if (size > 0)
		advance_pos(size);
	return (size != 0);
}

bool Lexer::compare_token(token *node)
{
	std::string ruleContent;
	bool		result;
	std::string::iterator start;

	ruleContent = _rules[node->value];
	if (ruleContent.empty() == true)
		return (false);
	start = start_pos();
	result = match_rule("", ruleContent);
	if (result == true)
	{
		node->input_value = std::string(start, _pos);
		
	}
	else
		_pos = start;
	return (result);
}

bool Lexer::compare_special(token *node)
{
	(void)node;
	std::string word;
	std::string::iterator start;
	std::string::iterator last;

	start = start_pos();
	last = _line.end();
	word = _input.nextWord(_pos, last);
	if (word.empty() == true)
		return (false);
	advance_pos(word.size());
	return (true);
}

bool Lexer::compare_choice(token *node)
{
	bool result;

	result = naviguate_in_tree(node->left);
	if (result == false)
		result = naviguate_in_tree(node->right);
	return (result);
}

bool Lexer::compare_concatenation(token *node)
{
	bool result;
	bool status;
	std::string rule_concatenation;

	result = false;
	status = _input.get_size_check();
	rule_concatenation = obtain_rule_concatenation(node);
	_input.size_check_on(false);
	result = match_rule("", rule_concatenation);
	_input.size_check_on(status);
	return (result);
}

std::string Lexer::obtain_rule_concatenation(token *node)
{
	std::string lhs;
	std::string rhs;
	std::string rule_concatenation;

	if (node->left != NULL)
		lhs = new_rule_from_nodes(node->left);
	if (node->right != NULL)
		rhs = new_rule_from_nodes(node->right);
	rule_concatenation = lhs + " " + rhs;
	return (rule_concatenation);
}

std::string Lexer::new_rule_from_nodes(token *node)
{
	std::string new_rule;

	if (node == NULL)
		return ("");
	new_rule = " ";
	if (node->left != NULL)
		new_rule += new_rule_from_nodes(node->left);
	if (node->type == TOKEN)
	{
		Rule token_rule(_rules[node->value]);
		new_rule += new_rule_from_nodes(token_rule.get_root());
	}
	else
		new_rule += node->original_value;
	if (node->right != NULL)
		new_rule += new_rule_from_nodes(node->right);
	return (new_rule);
}

bool Lexer::compare_repeat(token *node)
{
	bool result;
	bool status;
	std::string::iterator start;

	status = _input.get_size_check();
	_input.size_check_on(false);
	
	start = start_pos();
	result = match_rule("", node->value);
	node->input_value += std::string(start, _pos);
	node->input_value += " ";
	if (result == true)
	{
		start = start_pos();
		while (match_rule("", node->value) == true)
		{
			node->input_value += std::string(start, _pos);
			node->input_value += " ";
			start = start_pos();
		}
	}
	_input.size_check_on(status);
	return (result);
}

bool Lexer::compare_group(token *node)
{
	bool result;
	result = match_rule("", node->value);
	return (result);
}


bool Lexer::compare_optional(token *node)
{
	match_rule("", node->value);
	return (true);
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

void Lexer::set_input(std::string input) {
	_input = input;
}
Lexer::~Lexer() {}

Input 				&Lexer::get_input() { return (_input); }
const Input			&Lexer::get_input() const { return (_input); }

Vocabulary			&Lexer::get_rules() { return (_rules); }
const Vocabulary	&Lexer::get_rules() const { return (_rules); }

std::vector<KeyWord>		Lexer::get_list() { return (_list); }
const std::vector<KeyWord>	Lexer::get_list() const { return (_list); }


std::ostream& operator<<(std::ostream& out, const KeyWord& origin)
{
	std::vector<Token>::const_iterator it;
	out << "KeyWord: " << origin.tokenType  << std::endl;

	it = origin.args.begin();
	while (it != origin.args.end())
	{
		out << "               - "  << it->tokenType << " "  << it->valueToken << std::endl;
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

std::ostream& operator<<(std::ostream& out, const std::vector<KeyWord>& tokenList)
{

	std::vector<KeyWord>::const_iterator first;
	std::vector<KeyWord>::const_iterator last;
	first = tokenList.begin();
	last = tokenList.end();
	


	for (; first != last;  first++)
	{
		out << " " << *first << "\n";

	}
	return (out);
}

Lexer::Lexer(std::string grammarFileName, std::string inputFileName)
	: _rules(Vocabulary(grammarFileName)), _input(inputFileName)
{
	match_content.insert(std::make_pair(STRING, &Lexer::compare_string));
	match_content.insert(std::make_pair(TOKEN,  &Lexer::compare_token));
	match_content.insert(std::make_pair(CONCATENATION,  &Lexer::compare_concatenation));
	match_content.insert(std::make_pair(SPECIAL,  &Lexer::compare_special));
	match_content.insert(std::make_pair(GROUP,  &Lexer::compare_group));
	match_content.insert(std::make_pair(REPEAT, & Lexer::compare_repeat));
	match_content.insert(std::make_pair(CHOICE,  &Lexer::compare_choice));
	match_content.insert(std::make_pair(OPTIONAL, & Lexer::compare_optional));
}

Lexer::Lexer(const Vocabulary &vocabulary, const Input &otherInput)
	: _rules(vocabulary), _input(otherInput)
{
	match_content.insert(std::make_pair(STRING, &Lexer::compare_string));
	match_content.insert(std::make_pair(TOKEN,  &Lexer::compare_token));
	match_content.insert(std::make_pair(CONCATENATION,  &Lexer::compare_concatenation));
	match_content.insert(std::make_pair(SPECIAL,  &Lexer::compare_special));
	match_content.insert(std::make_pair(GROUP,  &Lexer::compare_group));
	match_content.insert(std::make_pair(REPEAT, & Lexer::compare_repeat));
	match_content.insert(std::make_pair(CHOICE,  &Lexer::compare_choice));
	match_content.insert(std::make_pair(OPTIONAL, & Lexer::compare_optional));
}

Lexer::Lexer(const Vocabulary &vocabulary, std::string InputContent)
	: _rules(vocabulary), _input()
{
	_input.set_content(InputContent);
	match_content.insert(std::make_pair(STRING, &Lexer::compare_string));
	match_content.insert(std::make_pair(TOKEN,  &Lexer::compare_token));
	match_content.insert(std::make_pair(CONCATENATION,  &Lexer::compare_concatenation));
	match_content.insert(std::make_pair(SPECIAL,  &Lexer::compare_special));
	match_content.insert(std::make_pair(GROUP,  &Lexer::compare_group));
	match_content.insert(std::make_pair(REPEAT, & Lexer::compare_repeat));
	match_content.insert(std::make_pair(CHOICE,  &Lexer::compare_choice));
	match_content.insert(std::make_pair(OPTIONAL, & Lexer::compare_optional));
}
