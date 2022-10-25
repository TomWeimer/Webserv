#include "../../include/http/Server.hpp"

Server::Server(){
    Socket listening(true, 8080);
    this->_listeningSockets.push_back(listening); // we should addapt it when we will have more port to listen
	this->initFdset();
}

Server::~Server(){
    std::cout << "server destructor called"<< std::endl;
    return;
}

void	Server::listen_connection(){
    // TODO - check how to define and handle the max number of connection 
    // should have a loop listening on each port of the socket vector
    this->_listeningSockets[0].listenPort(10);
}

void	Server::handle_connection(){ //TODO : handle timeout!!
	int		fdMax;
	int listener = this->_listeningSockets[0].getSocketFd();
	fdMax = listener; 
	int	yes = 1;

	if (setsockopt(listener, SOL_SOCKET ,SO_REUSEADDR ,&yes,sizeof yes) == -1){ //remove "adress already in use" error msg
		perror("setsockopt");
		exit(1);
	}

    while(1) 
    {
        std::cout << "------------------WAITNG FOR NEW CONNECTIONS-------------------" << std::endl;
		this->_readFds = this->_master;
		if (select(fdMax+1, &this->_readFds, NULL, NULL, NULL) == -1)
			this->perror_exit("select");
		for (int i = 0; i <= fdMax; i++){
			if (FD_ISSET(i, &this->_readFds)){
				if (i == listener) { // handle new connection
					Socket client(&this->_listeningSockets[0], false);
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

void Server::initFdset(){
	FD_ZERO(&this->_master);
	FD_ZERO(&this->_readFds);
	FD_SET(this->_listeningSockets[0].getSocketFd(), &this->_master);
}

void    Server::perror_exit(std::string str){
    perror(str.c_str());
    exit(EXIT_FAILURE);
}