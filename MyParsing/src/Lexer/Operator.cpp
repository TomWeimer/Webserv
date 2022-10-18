#include "Operator.hpp"


Operator is_operator(std::string str)
{
	Operator symbol;
	symbol = is_operator(str[0]);
	return (check_syntax_operator(symbol, str));
}

Operator is_operator(char c)
{
	size_t		pos;
	std::string EBNFSymbol;

	EBNFSymbol = "*-,|=;\'\"#()[]{}?";
	pos = EBNFSymbol.find(c);
	if (pos == std::string::npos)
		return (NONE);
	return (static_cast<Operator>(EBNFSymbol[pos]));
}

Operator check_syntax_operator(Operator symbol, std::string str)
{
	if (operator_come_in_pairs(symbol) == true)
		return (check_syntax_pair_operator(symbol, str));
	return (symbol);
}

bool operator_come_in_pairs(Operator symbol)
{
	if (symbol == FIRSTQUOTE || symbol == SECONDQUOTE)
		return (true);
	else if (symbol == COMMENT || symbol == GROUPSTART)
		return (true);
	else if (symbol == OPTIONALSTART || symbol == REPEATSTART || symbol == SPECIAL)
		return (true);
	return (false);
}

Operator check_syntax_pair_operator(Operator symbol, std::string str)
{
	char c = static_cast<char>(symbol);
	if (symbol == GROUPSTART || symbol == OPTIONALSTART || symbol == REPEATSTART)
		c++;
	if (str.find_first_of(c) == std::string::npos)
		return (NOTPAIR);
	return (symbol);
}



