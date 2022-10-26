#include "Logfile.hpp"
#include "Lexer.hpp"

int main()
{
	Lexer	lexer("./tests/grammar_files/simple.ebnf", "./tests/config_files/simple_tests/valid/simple.conf");
	Logfile lexerLog("./tests/logs/output/simple.log");

	lexerLog << lexer.lexeme();
	if (compare_file("./tests/logs/output/simple.log", "./tests/logs/test_values/simple.log") == true)
	{
		std::cerr << "test1: " << "[" << GREEN << "OK" << RESET << "]" << std::endl;
	}
	else
	{
		std::cerr << "test1: " << "[" << RED << "KO" << RESET << "]" << std::endl;
	}
}