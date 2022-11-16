#include "Cgi.hpp"

Cgi::Cgi(Server *server, Request *request)
{
	_server = server;
	_request = request;
	check_cgi_validity();
}

Cgi::~Cgi(){}

void Cgi::check_cgi_validity(){
	std::cerr << "CGI accepted: " << _request->_location->cgi.cgi_extension << std::endl;
	if (get_file_extention() == "php" || get_file_extention() == "py")
		_is_cgi = true;
	else
		_is_cgi = false;
	std::cout << "is cgi: " << _is_cgi << std::endl;
}

std::string Cgi::get_file_extention(){
	std::string file_extention;
	std::string file_path = _request->_target;
	size_t pos = file_path.find_last_of(".");
	if (pos != std::string::npos)
		file_extention = file_path.substr(pos + 1);
	std::cout << "file_extention: " << file_extention << std::endl;
	return file_extention;

}