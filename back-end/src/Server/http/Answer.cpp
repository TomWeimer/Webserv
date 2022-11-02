#include "Answer.hpp"

Answer::Answer(Request *request, Server* server)
		:_server(server), _request(request)
	{
	logInfo();
	_invalid_rout = (!_request->isValid());
	this->setFullAnswer();
	std::cerr << _request->getRout() << std::endl;
	if (this->_invalid_rout)
		std::cout << "INVALID ROUT" << std::endl;
	else
		std::cout << "VALID ROUT" << std::endl; //temporary
}

Answer::~Answer(){}


void Answer::setFullAnswer(){
	if (checkValidity()) {
		if (_request->getRequestType() == "GET")
			getRequest();
		else if (_request->getRequestType() == "DELETE")
			deleteRequest();
		else if (this->_request->getRequestType() == "POST")
			postRequest();
	}
}

bool Answer::checkValidity(){
	if (!_request->isHttpValid()){
		invalidRequest("400 Bad Request");
		return false;
	}
	else if(!_request->isMethodValid() && _request->isTargetValid()){ //wrong method but good location -> Forbidden
		invalidRequest("403 Forbidden");
		return false;
	}
	else if(!_request->isTargetValid() && _request->getRequestType() != "POST"){
		invalidRequest("404 Not Found");
		return false;
	}
	return true;
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
	std::ifstream testFile;
	testFile.open(this->_request->getRout().c_str());
	if (!testFile.is_open()){
		invalidRequest("403 Forbidden");
		return;
	}
	testFile.close();
	int result = std::remove(this->_request->getRout().c_str());
	if (!result)
		std::cout << "deleted" << std::endl;
	else
		perror("Remove");
	this->_body = "File deleted\n";
	std::string line = "HTTP/1.1 204 No Content\nContent-Type: text/plain\nContent-Length: ";
	std::string len = NumberToString(this->_body.size());
	this->_fullAnswer = line + len + "\n\n" + this->_body;
}

void Answer::postRequest(){ //TODO: check if file already exists
	std::ifstream testFile;
	testFile.open(this->_request->getRout().c_str());
	if (testFile.is_open()){
		std::cout << "file exist already" << std::endl;
		testFile.close();
		invalidRequest("403 Forbidden");
		return;
	}
	std::ofstream newFile;
	newFile.open (this->_request->getRout().c_str(), std::ofstream::out); // for cgi appending: ofs.open ("test.txt", std::ofstream::out | std::ofstream::app);
	newFile << this->_request->getBody();
	newFile.close();
	this->_body = "File created\n";
	std::string line = "HTTP/1.1 201 Created\nContent-Type: text/plain\nContent-Length: ";
	std::string len = NumberToString(this->_body.size());
	this->_fullAnswer = line + len + "\n\n" + this->_body;
}


void Answer::invalidRequest(std::string statusCode){
	std::string contentType = "Content-Type: text/html\n";
	if (statusCode == "404 Not Found")
		setBody("front-end/error-html/404.html");
	else if (statusCode == "403 Forbidden")
		setBody("front-end/error-html/403.html");
	else if (statusCode == "400 Bad Request")
		setBody("front-end/error-html/400.html");
	else
		setBody("front-end/error-html/400.html");
	std::string contentLen = "Content-Length: " + NumberToString(this->_body.size()) + "\n\n";
	this->_fullAnswer = "HTTP/1.1 " + statusCode + "\n"
						+ contentType + contentLen + this->_body;
}

void Answer::sendAnswer(){
	send(this->_request->getSocketFd(), this->_fullAnswer.c_str(), this->_fullAnswer.size(), 0);
	// close(this->_request->getSocketFd());
	std::cout << "------------------ Answer sent -------------------" << std::endl;
}

void Answer::logInfo(){
	std::cout << "rout: " << _request->getRout() << std::endl;
	std::cout << "method: " << _request->getRequestType()<< std::endl;
	std::cout << "final rout: " << _request->getRout()<< std::endl;
	std::cout << "http valid: " << _request->isHttpValid()<< std::endl;
	std::cout << "rout valid: " << _request->isTargetValid()<< std::endl;
	std::cout << "method valid: " << _request->isMethodValid()<< std::endl;
}


