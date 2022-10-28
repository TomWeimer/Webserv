#include "../../include/http/Request.hpp"

Request::Request(int socketFD, std::string fullRequest){
	this->_socketFd = socketFD;
	this->_fullRequest = fullRequest;
	this->setTokens();
	this->setRequestType();
	this->setRout();
}

Request::~Request(){}

void Request::setRequestType(){

	if (this->_tokens.size() > 0)	
		this->_requestType = this->_tokens[0];
	else
		return;
	std::cout << "method: " << this->_requestType << std::endl;
}

void Request::setRout(){
	if (this->_tokens.size() <= 0)
		return ;	
	this->_tokens[1].erase(0, 1);
	this->_rout = this->_tokens[1];
	std::cout << "ROUT: " << this->_rout << std::endl;
}

void Request::setTokens(){
	std::stringstream check1(this->_fullRequest);
	std::string		tmp;
    while(std::getline(check1, tmp, ' '))
    {
        this->_tokens.push_back(tmp);
    }
}

std::string Request::getRequestType(){
	return this->_requestType;
}

std::string Request::getRout(){
	return this->_rout;
}

int	Request::getSocketFd(){
	return this->_socketFd;
}

std::string	Request::getFullRequest(){
	return this->_fullRequest;
}