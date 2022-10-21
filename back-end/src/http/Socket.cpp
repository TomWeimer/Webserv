#include "Socket.hpp"

Socket::Socket(bool isListening): _isListening(isListening){
    if (this->_isListening){
        if ((this->_socketFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
            this->perrorExit("In socket");
        this->_PORT = 8080;
        this->_address.sin_family = AF_INET;
        this->_address.sin_addr.s_addr = INADDR_ANY;
        this->_address.sin_port = htons( this->_PORT );
        this->_addrlen = sizeof(this->_address);
        this->bindSocket();
    }
}

Socket::Socket( Socket * listeningSocket, bool isListening){
    if (isListening == false){
        if ((this->_socketFd = accept(listeningSocket->getSocketFd(), (struct sockaddr *)listeningSocket->getPtrAddress(), (socklen_t*)listeningSocket->getPtrAddrlen()))<0) 
                this->perrorExit("In accept");
        this->_isListening = false;
    }
}

Socket::~Socket(){}

void Socket::bindSocket(){
    	if (bind(this->_socketFd, (struct sockaddr *)&this->_address, sizeof(this->_address))<0)
        this->perrorExit("In bind");
        std::cout << "LISTENING TO PORT: " << this->_PORT << std::endl;
}

void Socket::listenPort(int queueLen){
    if (listen(this->_socketFd, queueLen) < 0)
        this->perrorExit("In listen");
}

void  Socket::receiveRequest(){
    char	buffer;
	char	*str_line;
	int		i;
	
	i = 0;
	str_line = strdup("");
	while (recv(this->_socketFd, &buffer, 1, 0) > 0){
		str_line = ft_strjoin(str_line, buffer);
	}
	i = strlen(str_line);
	if (i == 0){
		free(str_line);
		str_line = NULL;
	}
	this->_requestBuffer = str_line;
}

int Socket::getSocketFd(){
    return this->_socketFd;
}

int Socket::getAddrlen(){
    return this->_addrlen;
}

int *Socket::getPtrAddrlen(){
    return &this->_addrlen;
}

int Socket::getPort(){
    return this->_PORT;
}

struct	sockaddr_in Socket::getAddress(){
    return this->_address;
}

struct	sockaddr_in 	*Socket::getPtrAddress(){
    return &this->_address;
}


void    Socket::perrorExit(std::string str){
    perror(str.c_str());
    exit(EXIT_FAILURE);
}