#include "Server.hpp"

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

	for (it = _locationsList.begin(); it != _locationsList.end(); it++)
	{
		if (target == it->path)
			return &(*it);
	}
	return (&_info);
}

std::string Server::obtain_final_target(BlockParams *location, std::string target)
{
	std::string final_target;

	final_target = location->root;
	final_target += target;
	std::cerr << final_target << std::endl;
	return (final_target);
}

bool	Server::is_valid_target(std::string target)
{
	std::ifstream	file;

	file.open(target.c_str());
	if (!file.is_open())
		return (false);
	file.close();
	return (true);
}

void	Server::process_get(AnswerHeader* header, std::string& body, std::string target)
{
	body = obtain_body_content(target);
	header->add_header("Content-Length: " + NumberToString(body.size()));
	set_status_code(200);
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
	std::vector<std::string>::iterator it;

	for (it = location->allowed_methods.begin(); it != location->allowed_methods.end(); it++)
	{
		if (*it == method)
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

std::string NumberToString ( size_t Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}