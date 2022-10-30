#include "tests.hpp"
#include "config/Settings.hpp"


bool testRequestParsing()
{
//	std::cerr << Settings();
	execute_test("only parsing: ", &parsing_request);

	return (true);
}

bool parsing_request()
{
	Vocabulary			voc("./back-end/conf/HTTP.ebnf");
	
	std::string str = "GET /favicon.ico HTTP/1.1\nHost: localhost:8080";

	Lexer				lex(voc, str);
	std::stringstream	ssr;
	Logfile				log("./back-end/core/tests/logs/result/parsing_request.log");

	log.clear();
	ssr << lex.lexeme();
	log << ssr;
	return (compare_file("./back-end/core/tests/logs/result/parsing_request.log", "./back-end/core/tests/logs/expected/parsing_request.log"));
}
