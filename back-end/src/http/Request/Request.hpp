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
	std::string				file_extension;
	std::string				query;
	std::string				host;
	std::string				body;
	bool					size_limit_reached;	
	bool					chunked;
	BlockParams*			location;
	int						redirected;
};

#endif