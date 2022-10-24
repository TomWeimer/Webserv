#include "../../include/http/Answer.hpp"

Answer::Answer(Request *request){
	this->_invalid_rout = false;
	this->_request = request;
	this->setFullAnswer();
	if (this->_invalid_rout)
		std::cout << "INVALID ROUT" << std::endl;
	else
		std::cout << "VALID ROUT" << std::endl;
}

Answer::~Answer(){}

void Answer::setFullAnswer(){
	std::ifstream	file;
	std::string		file_content, line;
	std::string		full_answer;

	file.open(this->_request->getRout());
	if (!file.is_open()){
		this->_full_Answer = strdup("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 15\n\nPage not found!");
		this->_invalid_rout = true;
		return;
	} 
	else {
		while (std::getline(file, line)){
			file_content += line + "\n";
		}
		file.close();
		full_answer = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
					+ std::to_string(file_content.size()) + "\n\n" + file_content; 
		this->_full_Answer = strdup((char *)full_answer.c_str());
	}
}

void Answer::sendAnswer(){
	write(this->_request->getSocketFd(), this->_full_Answer, strlen(this->_full_Answer));
	close(this->_request->getSocketFd());
	std::cout << "------------------ Answer sent -------------------" << std::endl;
}
