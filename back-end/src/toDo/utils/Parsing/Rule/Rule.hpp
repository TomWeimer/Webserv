#ifndef RULE_HPP
#define RULE_HPP
#include <iostream>
#include <util.h>
#include <vector>
#include <sstream>
//#include "utils.hpp"
#include <fstream>

#define RESET "\033[0m"
#define BLACK "\033[30m"			  /* Black */
#define RED "\033[31m"				  /* Red */
#define GREEN "\033[32m"			  /* Green */
#define YELLOW "\033[33m"			  /* Yellow */
#define BLUE "\033[34m"				  /* Blue */

enum TOKEN_TYPE
{
	ROOT = 0,
	STRING = '\"',
	TOKEN = 'T',
	GROUP = 'G',
	REPEAT = 'R',
	OPTIONAL = 'O',
	GROUPSTART = '(',
	GROUPEND= ')',
	REPEATSTART= '{',
	REPEATEND= '}',
	OPTIONALSTART= '[',
	OPTIONALEND= ']',
	CONCATENATION= ',',
	CHOICE= '|',
	SPECIAL = '?',
	NONE
};

struct token
{
	TOKEN_TYPE	type;
	std::string	value;
	std::string	original_value;
	std::string input_value;
	std::vector<std::string> repeat;

	token		*left;
	token		*right;
	token()
	{
		type = NONE;
		value = "";
		left = NULL;
		right = NULL;
	}
	token(TOKEN_TYPE newType, std::string newValue)
	{
		type = newType;
		original_value = newValue;
		if (type == STRING || type == REPEAT || type == GROUP || type  == OPTIONAL)
		{
			value = newValue.substr(1, newValue.size() - 2);
		}
		else
		{
			value = newValue;
		}
		left = NULL;
		right = NULL;
	}
};



class Rule
{
	typedef std::pair< TOKEN_TYPE, std::string > ruleWord;
	private:
	std::string				_name;
	std::string				_rule;
	std::vector< std::pair< TOKEN_TYPE, std::string > > tokenList;
	token					*_root;

	private:
	void		create_rule();
	void		split_rule();
	void	format_rule();
	TOKEN_TYPE	obtain_token_type(std::string &tokenValue);
	void		create_tree();
	token *create_token(bool left);
	bool end_token(TOKEN_TYPE type);
	void obtain_full_token(std::stringstream& ssr, std::string& value, TOKEN_TYPE &type);

	void printBT(token *node);
	void printBT(const std::string& prefix, const token* node, bool isLeft);
	token		*create_token();
	std::pair<TOKEN_TYPE, std::string> new_token(TOKEN_TYPE type, std::string value);
	bool is_operator(TOKEN_TYPE type);
	void delete_tree(token *actualNode);
	std::string obtain_rule_side(token *node);

	

public:
	
	token *get_root();
	std::string obtain_rule_side();
	bool compare_value(std::string value);


	public:
	Rule();
	Rule(const Rule& otherRule);
	Rule& operator=(const Rule& otherRule);
	Rule(std::string rule, std::string rule_name = "");
	~Rule();



	public:
	std::string name();
	std::string					next_token();
	bool						empty();
		void printBT();
};

#endif