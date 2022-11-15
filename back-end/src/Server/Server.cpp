#include "Server.hpp"
#include <dirent.h>

Server::Server(std::vector<KeyWord> tokens)
	: _status_code(0)
{
	serverConfiguration(tokens);
	init_server_sockets();
}

void Server::serverConfiguration(std::vector<KeyWord> tokens)
{
	Config configuration(_info, _locationsList, tokens);
}

void Server::init_server_sockets()
{
	for (size_t i = 0; i < _info.port.size(); i++)
	{
		ServerSocket *new_socket = new ServerSocket(AF_INET, _info.port[i], _info.host[i]);
		_server_sockets.push_back(*new_socket);
	}
}


int Server::get_port(int i)
{
	return (_info.port[i]);
}

ServerBlock *Server::get_server_info()
{
	return (&_info);
}

std::vector<LocationBlock>& Server::get_location_list()
{
	return (_locationsList);
}

size_t Server::size()
{
	return (_server_sockets.size());
}

ServerSocket& Server::operator[](int index)
{
	return (_server_sockets[index]);
}

Server::~Server()
{
	std::cout << "server destructor called" << std::endl;
	return;
}

BlockParams *Server::find_location(std::string target)
{
	std::vector<LocationBlock>::iterator it;
	std::string target_request;
	std::string location_path;

	for (it = _locationsList.begin(); it != _locationsList.end(); it++)
	{
		if (it->root.empty() == true)
			location_path = target_request = _info.root;
		else
			location_path = target_request = it->root;
		location_path += it->path;
		target_request += target;
		std::cerr << target_request << " " <<  location_path << std::endl;
		if (std::strncmp(target_request.c_str(),  location_path.c_str(), location_path.size()) == 0)
			return &(*it);
	}
	return (&_info);
}

std::string Server::obtain_final_target(BlockParams *location, std::string target)
{
	std::string final_target;

	if (location->root.empty() == false)
		final_target = location->root;
	else
		final_target = _info.root;
	final_target += target;
	std::cerr << "final target: " <<  final_target << std::endl;
	return (final_target);
}

bool	Server::is_valid_target(std::string& target, BlockParams* location)
{

	std::cerr << "valid target: " << target << std::endl;
	if (isDir(target) == true)
	{
		if ((location->autoindex == NONE && _info.autoindex == ON) || location->autoindex == ON)
			return (true);
		else if (search_index(target, location) == false)
		{
			set_status_code(503);
			return (false);
		}

	}
	return (file_exists(target));
}

bool	Server::file_exists(std::string target)
{
	std::ifstream	file;

	file.open(target.c_str());
	if (!file.is_open()) {
		return (false);
	}
	file.close();
	return (true);
}


bool	Server::search_index(std::string& target, BlockParams* location)
{
	std::vector<std::string> *index_names;
	std::vector<std::string>::iterator it;
	std::string copy_target;

	copy_target = target;
	if (location->index.empty() == false)
		index_names = &location->index;
	else
		index_names = &_info.index;
	for (it = index_names->begin(); it != index_names->end(); it++)
	{
		if (file_exists(copy_target + *it) == true)
		{
			target += *it;
			return (true);
		}
	}
	return (false);

}


bool Server::isDir(std::string target)
{
	std::fstream fileOrDir(target.c_str());

	if (fileOrDir.is_open() == false)
	{
		return (true);
	}
	fileOrDir.close();
	return (false);
}

void	Server::process_get(AnswerHeader* header, std::string& body, std::string target, int directory_listing, std::string root)
{
	if (directory_listing == NONE && _info.autoindex != NONE)
		directory_listing = _info.autoindex;
	if (directory_listing == ON && isDir(target) == true)
		body = display_directory_listing(target, root);
	else
		body = obtain_body_content(target);
	header->add_header("Content-Length: " + NumberToString(body.size()));
	set_status_code(200);
}

std::string Server::display_directory_listing(std::string target, std::string root)
{
	DIR* directory;
	struct dirent *entry;
	std::string result;

	if (root.empty() == true)
		root = _info.root;

	result.append("<!DOCTYPE html>\n");
	result.append("<html lang=\"en\">\n");
	result.append("<head>\n");
	result.append("</head>\n");
	result.append("<body>\n");
	result.append("\t<h1>Index of " + target + " </h1>\n");

	directory = opendir(target.c_str());
	if (!directory)
		return "";
	std::vector<struct dirent> entries;
	while ((entry = readdir(directory)) != NULL)
	{
		entries.push_back(*entry);
	}

	for (std::vector<struct dirent>::iterator it = entries.begin(); it != entries.end(); it++)
	{
		std::string name(it->d_name);
		std::string line = "<a href=\"";
		line += target.substr(root.size());
		if (line[line.size() - 1] != '/')
			line += '/';
		line += it->d_name;
		line += "\">";
		line += it->d_name;
		line += "</a></br>\n";
		result += line;
	}
	result += "</body>\r\n";
	closedir(directory);
	return (result);
}


void	Server::process_post(AnswerHeader* header, std::string& body, std::string target)
{
	if (post_check_file_already_exist(target) == true)
	{
		header->add_header("Content-Length: " + NumberToString(0));
		return ;
	}

	std::ofstream newFile;

	newFile.open (target.c_str(), std::ofstream::out); // for cgi appending: ofs.open ("test.txt", std::ofstream::out | std::ofstream::app);
	newFile << body;
	newFile.close();
	set_status_code(201);
	body.clear();
	header->add_header("Content-Length: " +  NumberToString(body.size()));
	header->add_header("Location: " + target);
}

bool Server::post_check_file_already_exist(std::string target)
{
	std::ifstream testFile;

	testFile.open(target.c_str());
	if (testFile.is_open())
	{
		testFile.close();
		set_status_code(203);
		return (true);
	}
	return (false);
}

bool Server::delete_check_if_file_exist(std::string target)
{
	std::ifstream testFile;

	testFile.open(target.c_str());
	if (!testFile.is_open())
	{
		set_status_code(204);
		return (false);
	}
	testFile.close();
	return (true);
}



void	Server::process_delete(AnswerHeader* header, std::string& body, std::string target)
{
	if (delete_check_if_file_exist(target) != false) // 204 No content
	{
		if (std::remove(target.c_str())) // 200 OK
		{
			set_status_code(200);
			body = "<html>\n  <body>\n    <h1>File deleted.</h1>\n  </body>\n</html>";
		}
		else
			set_status_code(202); // 202 Accepted
	}
	header->add_header("Content-Length: " + NumberToString(body.size()));
}


std::string Server::obtain_body_content(std::string target)
{
	std::ifstream	file;
	std::string		file_content, line;

	file.open(target.c_str());
	while (std::getline(file, line)){
		file_content += line + "\n";
	}
	file.close();
	return (file_content);
}

bool Server::is_valid_method(std::string method, BlockParams* location)
{
	std::vector<std::string>::iterator first;
	std::vector<std::string>::iterator last;

	if (location->allowed_methods.empty() == true && _info.allowed_methods.empty() == true)
	{
		return (true);
	}
	if (location->allowed_methods.empty() == false)
	{
		first = location->allowed_methods.begin();
		last = location->allowed_methods.end();
	}
	else
	{
		first = _info.allowed_methods.begin();
		last = _info.allowed_methods.end();	
	}
	for (; first != last; first++)
	{
		if (*first == method)
			return (true);
	}
	return (false);
}

void Server::set_status_code(int number)
{
	if (_status_code == 0)
		_status_code = number;
}
int Server::get_status_code()const
{
	return (_status_code);
}

bool Server::no_error()
{
	return (_status_code == 0);
}

void Server::reset_status_code()
{
	_status_code = 0;
}

std::string Server::error_page()
{
	std::string content;

	if (_info.error_pages.find(_status_code) == _info.error_pages.end())
		return (content);
	return (obtain_body_content(_info.error_pages[_status_code]));
}

std::string NumberToString ( size_t Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}