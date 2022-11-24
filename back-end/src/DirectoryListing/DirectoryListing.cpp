#include "DirectoryListing.hpp"

std::string DirectoryListing::obtain_directory_content(std::string target, std::string root)
{
	obtain_directory_info(target);
	create_links_to_entries(target, root);
	return (build_html_page(target));
}

void DirectoryListing::obtain_directory_info(std::string target)
{
	_directory = opendir(target.c_str());
	while ((_actualEntry = readdir(_directory)) != NULL)
	{
		_entries.push_back(*_actualEntry);
	}
	closedir(_directory);
}

void DirectoryListing::create_links_to_entries(std::string target, std::string root)
{
	std::vector<entry_t>::iterator	it;
	std::string						entry_name;
	std::string						link;

	target = format_target(target, root.size());
	for (it = _entries.begin(); it != _entries.end(); it++)
	{
		entry_name = it->d_name;
		link = "<a href=\"" + target + entry_name + "\">" + entry_name + "</a></br>\n";
		_links += link;
	}
}

std::string	DirectoryListing::format_target(std::string target, size_t root_size)
{
	std::string target_without_root;

	target_without_root = target.substr(root_size);
	if (target_without_root[target_without_root.size() - 1] != '/')
		target_without_root += '/';
	return (target_without_root);
}

std::string DirectoryListing::build_html_page(std::string target)
{
	std::string html;

	html += "<!DOCTYPE html>\n";
	html += "<html lang=\"en\">\n";
	html += "<head>\n";
	html += "</head>\n";
	html += "<body>\n";
	html += "\t<h1>Index of " + target + " </h1>\n";
	html += _links;
	html += "</body></html>";
	return (html);
}
