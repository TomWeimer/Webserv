#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "../../Server/Settings/Settings.hpp"
#include <vector>

struct Request
{
public:
	std::string				line;
	std::string				method;
	std::string				target;
	std::string				version;
	std::string				host;
	std::string				body;
	BlockParams*			location;
	int						redirected;
};

#endif