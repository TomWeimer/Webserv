#include "../../include/http/Answer.hpp"

Answer::Answer(Request *request){
	this->_request = request;
	this->checkRout();
	this->setFullAnswer();
	if (this->_invalid_rout)
		std::cout << "INVALID ROUT" << std::endl;
	else
		std::cout << "VALID ROUT" << std::endl;
}

Answer::~Answer(){}


void Answer::setFullAnswer(){
	if (this->_invalid_rout){
		this->invalidRequest("404 Not Found");
		return;
	} else {
		getRequest();
	}
}

void Answer::setBody(std::string bodyLocation){
	std::ifstream	file;
	std::string		file_content, line;
	file.open(bodyLocation);
	while (std::getline(file, line)){
		file_content += line + "\n";
	}
	file.close();
	this->_body = file_content;
}

void Answer::getRequest(){
	this->setBody(this->_request->getRout());
	std::string line = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	std::string len = std::to_string(this->_body.size());
	this->_fullAnswer = line + len + "\n\n" + this->_body;
}

void Answer::invalidRequest(std::string statusCode){
	std::string contentType = "Content-Type: text/html\n";
	if (statusCode == "404 Not Found")
		setBody("front-end/html/404.html");
	else
		setBody("front-end/html/404.html");
	std::string contentLen = "Content-Length: " + std::to_string(this->_body.size()) + "\n\n";
	this->_fullAnswer = "HTTP/1.1 " + statusCode + "\n"
						+ contentType + contentLen + this->_body;
}

void Answer::sendAnswer(){
	send(this->_request->getSocketFd(), this->_fullAnswer.c_str(), this->_fullAnswer.size(), 0);
	close(this->_request->getSocketFd());
	std::cout << "------------------ Answer sent -------------------" << std::endl;
}

void Answer::checkRout(){ //temporary
	std::ifstream	file;

	file.open(this->_request->getRout());
	if (!file.is_open()){
		this->_invalid_rout = true;
		return;
	} 
	else {
		file.close();
		this->_invalid_rout = false;
	}
}

