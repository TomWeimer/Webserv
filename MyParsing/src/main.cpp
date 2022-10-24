#include "Vocabulary.hpp"
#include "Lexer2.hpp"
#include "Input.hpp"
#include "config/Settings.hpp"

int main()
{
	Lexer lexer("rules.ebnf", "input.conf");
	Settings OUF (lexer.lexeme());

	std::cerr << OUF << std::endl;
}
// std::vector<KeyWord> v;
// 	std::vector<KeyWord>::iterator first;
// 	std::vector<KeyWord>::iterator last;
	

	
// 	v = lexer.lexeme();
// 	first = v.begin();
// 	last = v.end();
// 	std::cout << " " << v.back().tokenType << std::endl;
// 	for (; first != last;  first++)
// 	{
// 		std::cout << " " << *first << std::endl;

// 	}