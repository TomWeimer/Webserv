#include "Vocabulary.hpp"
#include "Lexer.hpp"
#include "Input.hpp"

int main()
{
	Lexer lexer("rules.ebnf", "input.conf");
	std::cerr << lexer.lexeme();
//	std::cerr << "OUIOUT" << std::endl;

	//std::cerr << OUF << std::endl;
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