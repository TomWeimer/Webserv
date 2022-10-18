#ifndef OPERATOR_HPP
#define OPERATOR_HPP
#include <iostream>
#include <sstream>

enum Operator
{
	REPETITION 		= '*',
	EXCLUSION 		= '-',
	CONCATENATION	= ',',
	CHOICE			= '|',
	DEFINITION		= '=',
	TERMINATION		= ';',
	FIRSTQUOTE		= '\'',
	SECONDQUOTE		= '\"',
	COMMENT			= '#',
	GROUPSTART		= '(',
	GROUPEND		= ')',	
	OPTIONALSTART	= '[',
	OPTIONALEND		= ']',
	REPEATSTART		= '{',
	REPEATEND		= '}',
	SPECIAL			= '?',
	NONE			= -1,
	NOTPAIR			= -2,
	SYNTAX_ERROR	= -3
};


Operator is_operator(std::string str);
Operator is_operator(char c);
Operator check_syntax_operator(Operator symbol, std::string str);
Operator check_syntax_pair_operator(Operator symbol, std::string str);
bool 	 operator_come_in_pairs(Operator symbol);


#endif