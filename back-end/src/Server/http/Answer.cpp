#include "Answer.hpp"

Answer::Answer(Request *request, Server* server)
		:_server(server), _request(request)
	{
	_invalid_rout = (!_request->is_valid());
	this->setFullAnswer();
	std::cerr << _request->getRout() << std::endl;
	if (this->_invalid_rout)
		std::cout << "INVALID ROUT" << std::endl;
	else
		std::cout << "VALID ROUT" << std::endl; //temporary
}

Answer::~Answer(){}


void Answer::setFullAnswer(){
	if (this->_invalid_rout && this->_request->getRequestType() != "POST"){
		this->invalidRequest("404 Not Found");
		return;
	} else {
		if (this->_request->getRequestType() == "GET")
			this->getRequest();
		else if (this->_request->getRequestType() == "DELETE")
			this->deleteRequest();
		else if (this->_request->getRequestType() == "POST")
			this->postRequest();
	}
}

void Answer::setBody(std::string bodyLocation){
	std::ifstream	file;
	std::string		file_content, line;
	
	file.open(bodyLocation.c_str());
	while (std::getline(file, line)){
		file_content += line + "\n";
	}
	file.close();
	this->_body = file_content;
}

void Answer::getRequest(){
	std::string target;

	target += this->_request->getRout();

	this->setBody(target);
	std::string line = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	std::string len = NumberToString(this->_body.size());
	this->_fullAnswer = line + len + "\n\n" + this->_body;
}

void Answer::deleteRequest(){
	int result = std::remove(this->_request->getRout().c_str());
	if (!result)
		std::cout << "deleted" << std::endl;
	else
		perror("Remove");
	this->_body = "File deleted\n";
	std::string line = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
	std::string len = NumberToString(this->_body.size());
	this->_fullAnswer = line + len + "\n\n" + this->_body;
}

void Answer::postRequest(){ //TODO: check if file already exists
	//need to have a parser for the body
}


void Answer::invalidRequest(std::string statusCode){
	std::string contentType = "Content-Type: text/html\n";
	if (statusCode == "404 Not Found")
		setBody("front-end/error-html/404.html");
	else
		setBody("front-end/error-html/404.html");
	std::string contentLen = "Content-Length: " + NumberToString(this->_body.size()) + "\n\n";
	this->_fullAnswer = "HTTP/1.1 " + statusCode + "\n"
						+ contentType + contentLen + this->_body;
}

void Answer::sendAnswer(){
	send(this->_request->getSocketFd(), this->_fullAnswer.c_str(), this->_fullAnswer.size(), 0);
	// close(this->_request->getSocketFd());
	std::cout << "------------------ Answer sent -------------------" << std::endl;
}


