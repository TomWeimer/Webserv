#ifndef TESTS_HPP
#define TESTS_HPP
#include <iostream>
#include "Logfile.hpp"
#include "Lexer.hpp"

#define RESET   "\033[0m"  
#define RED     "\033[31m"  
#define GREEN   "\033[32m"  

bool execute_test(std::string testName, bool (*testFunction)(void));
bool print_result( bool (*testFunction)(void));
bool testSettings();
bool default_Settings();
bool custom_Settings();
bool parsing_request();
bool testRequestParsing();
#endif