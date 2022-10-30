#include "tests.hpp"


bool print_result( bool (*testFunction)(void))
{
	bool result;
	if ((result = testFunction()) == true)
		std::cerr << GREEN << " ok" << RESET << std::endl;
	else
		std::cerr << RED << " ko" << RESET << std::endl;
	return (result);
}

bool execute_test(std::string testName, bool (*testFunction)(void))
{
	std::cerr << testName << ":";
	return (print_result(testFunction));
}