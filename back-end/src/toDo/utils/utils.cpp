#include "utils.hpp"

std::string NumberToString ( size_t Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}