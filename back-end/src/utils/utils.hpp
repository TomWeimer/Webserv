#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <dirent.h>
bool	file_exists(std::string target);
bool	isDir(std::string target);
std::string obtain_body_content(std::string target);
std::string NumberToString ( size_t Number );
int StrToInt(std::string str);

#endif