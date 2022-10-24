#include "Operator.hpp"


Operator is_operator(std::string str)
{
	Operator symbol;
	symbol = is_operator(str[0]);
	return (check_syntax_operator(symbol, str));
}

std::string operator_list()
{
	std::string EBNFSymbol;

	EBNFSymbol = "*-,|=;<>\'\"#()[]{}?";
	return (EBNFSymbol);
}

Operator is_operator(char c)
{
	size_t		pos;
	std::string EBNFSymbol;

	EBNFSymbol = "*-,|=;<>\'\"#()[]{}?";
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
	if (symbol == FIRSTQUOTE || symbol == SECONDQUOTE || symbol == TOKENSTART)
		return (true);
	else if (symbol == COMMENT )
		return (true);
	return (false);
}

Operator check_syntax_pair_operator(Operator symbol, std::string str)
{
	char c = static_cast<char>(symbol);
	if (symbol == GROUPSTART || symbol == OPTIONALSTART || symbol == REPEATSTART)
		c++;
	else if (symbol == TOKENSTART)
		c += 2;
	if (str.find_first_of(c) == std::string::npos)
		return (NOTPAIR);
	return (symbol);
}



