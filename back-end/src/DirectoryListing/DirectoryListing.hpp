#ifndef DIRECTORYLISTING_HPP
#define DIRECTORYLISTING_HPP
#include <dirent.h>
#include <iostream>
#include <vector>
class DirectoryListing
{
	typedef struct dirent entry_t;
private:
	DIR*					_directory;
	entry_t*				_actualEntry;
	std::vector<entry_t>	_entries;
	std::string				_links;

public:
	DirectoryListing() {}
	~DirectoryListing() {}

public:
	std::string obtain_directory_content(std::string target, std::string root);

private:
	void		obtain_directory_info(std::string target);
	void		create_links_to_entries(std::string target, std::string root);
	std::string	format_target(std::string target, size_t root_size);
	std::string build_html_page(std::string target);
};
#endif