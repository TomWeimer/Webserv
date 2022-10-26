#include "../../include/http/Server.hpp"

Server::Server(){
	this->_timeout.tv_sec = 2;
	this->_timeout.tv_usec = 500000;
	this->initSocket();
	this->initFdset();
}

Server::~Server(){
    std::cout << "server destructor called"<< std::endl;
    return;
}

void Server::initSocket(){
	for (int i = 0; i < (int)this->_settingsInfo.get_serverInfo().port.size(); i++){
		Socket *listening = new Socket(true, this->_settingsInfo.get_serverInfo().port[i]);
	    this->_listeningSockets.push_back(*listening); 
	}
}

void	Server::listen_connection(){
    // TODO - check how to define and handle the max number of connection 
    // should have a loop listening on each port of the socket vector
	for (int i = 0; i < (int)this->_listeningSockets.size(); i++){
		std::cout << "listening on PORT:  "  << this->_settingsInfo.get_serverInfo().port[i] << std::endl;
   		this->_listeningSockets[i].listenPort(10);
	}
}

void	Server::handle_connection(){
	int		fdMax;
	// int listener = this->_listeningSockets[0].getSocketFd();
	fdMax = this->maxListenerFd(); 


	this->socketOption(); //remove "adress already in use" error msg
    while(1) 
    {
        std::cout << "---------WAITNG FOR NEW CONNECTIONS... (timeout = 2.5 seconde) -----------" << std::endl;
		this->_readFds = this->_master;
		if (select(fdMax+1, &this->_readFds, NULL, NULL, &this->_timeout) == -1)
			this->perror_exit("select");
		for (int i = 0; i <= fdMax; i++){
			if (FD_ISSET(i, &this->_readFds)){
				if (this->isListener(i)) { // handle new connection
					Socket client(this->findListenerFd(i), false);
					if (client.getSocketFd() == -1)
						perror("accept");
					else{
						FD_SET(client.getSocketFd(), &this->_master);
						if (client.getSocketFd() > fdMax)
							fdMax = client.getSocketFd();
				}
				} else { //handle message from client
					std::string fileStr = this->recvMessage(i);
					Request request(i, fileStr);
					Answer answer(&request);
					answer.sendAnswer();
					close(i);
					FD_CLR(i, &this->_master);
				}
			}
		}
    }
}

std::string Server::recvMessage(int socketFd){
	char *buf = (char*)calloc(200, sizeof(char));
	std::string fileStr;
	int nDataLength;

	while ((nDataLength = recv(socketFd, buf, 200, 0)) > 0) {
		fileStr.append(buf, nDataLength);
		if (buf[nDataLength - 1] == '\n')
			break;
		bzero((void*)buf, 200);
	}
	return fileStr;
}

void Server::socketOption(){ //remove "adress already in use" error msg
	int yes = 1;

	for (int i = 0; i < (int)this->_listeningSockets.size(); i++){
		if (setsockopt(this->_listeningSockets[i].getSocketFd(), SOL_SOCKET ,SO_REUSEADDR ,&yes,sizeof yes) == -1){ //remove "adress already in use" error msg
			perror("setsockopt");
			exit(1);
		}
	}
}
void Server::initFdset(){
	FD_ZERO(&this->_master);
	FD_ZERO(&this->_readFds);
	for (int i = 0; i < (int)this->_listeningSockets.size(); i++){
		FD_SET(this->_listeningSockets[i].getSocketFd(), &this->_master);
		std::cout << this->_listeningSockets[i].getSocketFd() << std::endl;
	}
}

bool Server::isListener(int socketFd){
	for (int i = 0; i < (int)this->_listeningSockets.size(); i++){
		if (socketFd == this->_listeningSockets[i].getSocketFd())
			return true;
	}
	return false;
}

int Server::maxListenerFd(){
	int maxFd = this->_listeningSockets[0].getSocketFd();
	for (int i = 0; i < (int)this->_listeningSockets.size(); i++){
		if (this->_listeningSockets[i].getSocketFd() > maxFd)
			maxFd = this->_listeningSockets[i].getSocketFd();
	}
	return maxFd;
}

Socket *Server::findListenerFd(int socketFd){
	for (int i = 0; i < (int)this->_listeningSockets.size(); i++){
		if (socketFd == this->_listeningSockets[i].getSocketFd())
			return &this->_listeningSockets[i];
	}
	return &this->_listeningSockets[0];
}

void    Server::perror_exit(std::string str){
    perror(str.c_str());
    exit(EXIT_FAILURE);
}