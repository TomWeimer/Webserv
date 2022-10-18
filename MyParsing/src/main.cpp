#include "Vocabulary.hpp"
#include "Lexer.hpp"
#include "Input.hpp"

int main()
{
	Vocabulary rulesToken("rules.ebnf");
	Lexer lexer(rulesToken);
	std::vector<std::string>::iterator first;
	std::vector<std::string>::iterator last;
	std::vector<std::string> v;
	
	v = lexer.lexeme("input.conf");
	first = v.begin();
	last = v.end();
	for (; first != last;  first++)
	{
		std::cerr << "HELLO" << std::endl;
		std::cout << *first << std::endl;
	}
}