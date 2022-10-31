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

	file.open(this->_request->getRout().c_str());
	if (!file.is_open()){
		this->_fullAnswer = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 15\n\nPage not found!";
		this->_invalid_rout = true;
		return;
	} 
	else {
		while (std::getline(file, line)){
			file_content += line + "\n";
		}
		file.close();
		this->_fullAnswer = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
					+ NumberToString(file_content.size()) + "\n\n" + file_content; 
	}
}

  std::string Answer::NumberToString ( size_t Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

void Answer::sendAnswer(){
	send(this->_request->getSocketFd(), this->_fullAnswer.c_str(), this->_fullAnswer.size(), 0);
	close(this->_request->getSocketFd());
	std::cout << "------------------ Answer sent -------------------" << std::endl;
}
