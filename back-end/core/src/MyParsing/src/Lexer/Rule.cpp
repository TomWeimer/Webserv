#include "../../include/Rule.hpp"
#include<cstdlib>
Rule::Rule(std::string rule, std::string rule_name)
	:  _name(rule_name), _rule(rule)
{
	while (std::isspace(_rule[_rule.size() - 1]) == true )
	{
		_rule.erase(_rule.size() - 1, 1);
	}
	//_rule.erase(i, (_rule.size() - 1) - i);
	create_rule();
}

void Rule::create_rule()
{
	format_rule();
	split_rule();
	create_tree();
}

void Rule::format_rule()
{
	std::string operatorList;
	char		c;
	char		op;
	size_t pos;

	operatorList = ",|<>(){}\"[]?";
//	std::cerr <<"invite: " << _rule	<< std::endl;
	for (size_t i = 0; i < _rule.size(); i++)
	{
		c = _rule[i];
		
		if (std::isspace(c) == false)
		{
			pos = operatorList.find(c);
			if (pos != std::string::npos)
			{
				op = operatorList[pos];
				if (op == STRING)
				{
					i++;
					while (_rule[i] != '\"')
					{
						i++;
					}
				}
				else if (op == ',' || op == '|' || op == '{' || op == '}'
					|| op == '(' || op == ')' || op == '[' || op == ']'
					|| op == '<'|| op == '>' || op == '?')
				{
					if (op != '<' && i + 1 < _rule.size() - 1)
					{
						_rule.insert(i + 1, " ");
					}
					if (op != '>')
					{
					_rule.insert(i, " ");
					i++;
					}
				}
			}
		}
		
	}
	//std::cerr <<"invite: " << _rule	<< std::endl;

}

void Rule::split_rule()
{
	std::stringstream	ssr(_rule);
	std::string			tokenValue;
	std::string			tmp;
	TOKEN_TYPE			tokenType;

	while (ssr.eof() == false)
	{
		
		ssr >> tokenValue;
		tokenType = obtain_token_type(tokenValue);
		if (end_token(tokenType) == false)
		{
			obtain_full_token(ssr, tokenValue, tokenType);
		}
		if (tokenType != NONE && tokenValue.empty() == false)
		{
			
			tokenList.push_back(new_token(tokenType, tokenValue));
		}
		
	}
}


bool Rule::end_token(TOKEN_TYPE type)
{
	if (type == STRING || type == TOKEN || type == CONCATENATION || type == CHOICE)
		return (true);
	if (type == NONE)
		return (true);
	return (false);
}

void Rule::obtain_full_token(std::stringstream& ssr, std::string& value, TOKEN_TYPE& type)
{
	std::string		op_start;
	std::string		op_end;
	std::string		word;
	int				group_nb;

	group_nb = 1;
	if (type == REPEATSTART)
	{
		type  = REPEAT;
		op_end = "}";
		op_start = "{";
	}
	else if (type == GROUPSTART)
	{
		type  = GROUP;
		op_end = ")";
		op_start = "(";
	}
	else if (type == OPTIONALSTART)
	{
		type  = OPTIONAL;
		op_end = "]";
		op_start = "[";
	}
	else if (type == SPECIAL)
	{
		type  = SPECIAL;
		op_end = "?";
		op_start = "";
	}
	while (ssr.eof() == false && group_nb > 0)
	{
		ssr >> word;

			//std::cerr << "word: " << word << std::endl;
		if (word == op_start)
			group_nb++;
		else if (word == op_end)
			group_nb--;
		value += " ";
		value += word;
	}
	//std::cerr << "v: " << value << std::endl;
}

std::string Rule::name()
{
	return (_name);
}

TOKEN_TYPE Rule::obtain_token_type(std::string &tokenValue)
{
	if (tokenValue[0] == ',')
		return (CONCATENATION);
	else if (tokenValue[0] == '|')
		return (CHOICE);
	else if (tokenValue[0] == '<' && tokenValue[tokenValue.size() - 1] == '>')
		return (TOKEN);
	else if (tokenValue[0] == '(')
		return (GROUPSTART);
	else if (tokenValue[0] == ')')
		return (GROUPEND);
	else if (tokenValue[0] == '\"' && tokenValue[tokenValue.size() - 1] == '\"')
		return (STRING);
	else if (tokenValue[0] == '{')
		return (REPEATSTART);
	else if (tokenValue[0] == '}')
		return (REPEATEND);
	else if (tokenValue[0] == '[')
		return (OPTIONALSTART);
	else if (tokenValue[0] == ']')
		return (OPTIONALEND);
	else if (tokenValue[0] == '?')
		return (SPECIAL);
	return NONE;
}

std::pair<TOKEN_TYPE, std::string> Rule::new_token(TOKEN_TYPE type, std::string value)
 {
// 	size_t pos;

// 	if (type == STRING)
// 	{
// 		// pos = value.find_first_of('\"');
// 		// value.erase(pos, 1);
// 		// pos = value.find_first_of('\"');
// 	//	value.erase(pos, 1);
// 	}

	return (std::make_pair(type, value));
}

void Rule::create_tree()
{
	ruleWord actualWord;
	//std::cerr << tokenList.size() << std::endl;
	
	if (tokenList.empty() == false)
	{
		//actualWord
		_root = create_token(false);
		// _root->right = NULL;
		// if (is_operator(tokenList.front().first) == true)
		// {
		// 	_root->right = create_token();
		// }
		// else
		// {
		// 	_root->right = NULL;
		// }

		
		
	}
}

bool Rule::is_operator(TOKEN_TYPE type)
{
	return (!(type == NONE || type == TOKEN || type == STRING || type == GROUPEND));
}

token *Rule::create_token(bool is_left)
{
	token		*newToken;
	ruleWord	word;
	ruleWord	nextWord;

	if (tokenList.empty() == true)
		return (NULL);
	newToken = NULL;
	word = tokenList.front();
	tokenList.erase(tokenList.begin());
	//if (tokenList.empty() == true)
	nextWord = tokenList.front();
	if (nextWord.first == CONCATENATION || nextWord.first == CHOICE)
	{
		tokenList.erase(tokenList.begin());
		newToken = new token(nextWord.first, nextWord.second);
		newToken->left = new token(word.first, word.second);
		newToken->right = create_token(false);
	}
	else
	{
		newToken = new token(word.first, word.second);
		if (is_left == true)
		{
			newToken->right = NULL;
			newToken->left = NULL;
		}
		else
		{
			newToken->right = create_token(false);
			newToken->left = create_token(true);
		}
	}
	
	return (newToken);
}


void Rule::printBT()
{
	printBT(_root); 
}  

void Rule::printBT(token *node)
{
	printBT("", node, false); 
}  

void Rule::printBT(const std::string& prefix, const token * node, bool isLeft)
{
    if( node != NULL)
    {
        std::cerr << prefix;
        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
		if (node->type == ROOT)
      	  std::cout <<"( " << BLUE << "ROOT" << RESET  << " : " << YELLOW <<  node->value  << RESET<< ")" << std::endl;
		else
			 std::cout <<"( " << BLUE << static_cast<char>(node->type) << RESET  << " : " << YELLOW <<  node->value  << RESET<< ")" << std::endl;

        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->left, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}
Rule::~Rule()
{
	delete_tree(_root);
}

void Rule::delete_tree(token *actualNode)
{
	if (actualNode != NULL)
	{
		delete_tree(actualNode->left);
		delete_tree(actualNode->right);
	}
	if (actualNode != NULL)
		delete(actualNode);
}



token *Rule::get_root()
{
	return (_root);
}



