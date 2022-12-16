#include <iostream>
#include <cstring>

int StrToInt(std::string str)
{
	long result;

	result = std::strtol(str.c_str(), NULL, 16);
	std::cout << result << std::endl;
	return (static_cast<int>(result));
}

int main()
{
	std::string lol = "0x3F";
	 std::cout << StrToInt(lol) << std::endl;
}