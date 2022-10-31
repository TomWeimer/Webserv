#include "../../include/http/Request.hpp"
#include "Lexer.hpp"

Request::Request(int socketFD, std::string fullRequest){
	this->_socketFd = socketFD;
	this->_fullRequest = fullRequest;
	this->setTokens();
	this->setRequestType();
	this->setRout();
}

Request::~Request(){}

void Request::setRequestType(){
	this->_rout = this->_tokens[0];
	std::cout << "ROUT: " << this->_rout << std::endl;
}

void Request::setRout(){
	this->_tokens[1].erase(0, 1);
	this->_rout = this->_tokens[1];
	std::cout << "ROUT: " << this->_rout << std::endl;
}

void Request::setTokens(){
	std::stringstream check1(this->_fullRequest);
	std::string		tmp;
	size_t pos;
	Vocabulary tokenHeader("./back-end/conf/HTTP.ebnf");
	//std::cerr << this->_fullRequest << std::endl;
	while((pos =_fullRequest.find_first_of('\r')) != std::string::npos)
	{
		_fullRequest.erase(pos, 1);
	}
	std::cerr << this->_fullRequest << std::endl;
	Lexer lex(tokenHeader, this->_fullRequest);
	
	std::cerr << lex.lexeme() << std::endl;
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