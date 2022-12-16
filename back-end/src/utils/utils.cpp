#include "utils.hpp"
bool	file_exists(std::string target)
{
	std::ifstream	file;

	file.open(target.c_str());
	if (!file.is_open()) {
		return (false);
	}
	file.close();
	return (true);
}

bool	isDir(std::string target)
{
	DIR *directory;

	if (target.empty() == true)
		return (false);
	directory = opendir(target.c_str());
    if (directory != NULL)
    {
       closedir(directory);
	   return (true);
    }
	return (false);
}

std::string obtain_body_content(std::string target)
{
	std::ifstream	file;
	std::string		file_content, line;

	file.open(target.c_str());
	while (std::getline(file, line)){
		file_content += line;
		if (file.eof() != true)
			file_content += "\n";
	}
	file.close();
	return (file_content);
}

std::string NumberToString( size_t Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

int StrToInt(std::string str)
{
	long result;

	result = std::strtol(str.c_str(), NULL, 10);
	return (static_cast<int>(result));
}

int StrToHex(std::string str)
{
	long result;

	result = std::strtol(str.c_str(), NULL, 16);
	return (static_cast<int>(result));
}
