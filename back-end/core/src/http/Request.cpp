#include "http/Request.hpp"

Request::Request(int socketFD, char *fullRequest){
	this->_socket_fd = socketFD;
	this->_full_request = fullRequest;
	// this->setFullRequest();
	this->setRequestType();
	this->setRout();
}

Request::~Request(){}

void Request::setFullRequest(){
	this->_full_request = get_file(this->_socket_fd);
	std::cout << "FULL REQUEST: " << this->_full_request << std::endl;
}

void Request::setRequestType(){
	char *request_copy = strdup(this->_full_request);
	char *token = strtok(request_copy, " ");
	std::cout << "REQUEST TYPE: " << token << std::endl;
	this->_request_type = token;
	free(request_copy);
}

void Request::setRout(){
	char *request_copy = strdup(this->_full_request);
	char *token = strtok(request_copy, " ");
	token = strtok(NULL, " ");
	if (strlen(token) > 1)
		token++;
	this->_rout = token;
	std::cout << "REQUEST ROUT: " << token << std::endl;
}

std::string Request::getRequestType(){
	return this->_request_type;
}

std::string Request::getRout(){
	return this->_rout;
}

int	Request::getSocketFd(){
	return this->_socket_fd;
}