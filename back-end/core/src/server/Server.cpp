#include "../../include/http/Server.hpp"

Server::Server(){
    Socket listening(true);
    this->_listeningSockets.push_back(listening); // we should addapt it when we will have more port to listen
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

void	Server::handle_connection(){
	fd_set	master, read_fds;
	int		fdMax;//, nbytes;
	socklen_t addrlen;
	int listener = this->_listeningSockets[0].getSocketFd();
	int newfd;
	struct sockaddr_storage remoteaddr; // client address
	char *buf = (char*)malloc(sizeof(char) * 200);
	bzero((void*)buf, 200);
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	FD_SET(listener, &master);
	fdMax = listener; 
	int	yes = 1;

	if (setsockopt(listener, SOL_SOCKET ,SO_REUSEADDR ,&yes,sizeof yes) == -1){
		perror("setsockopt");
		exit(1);
	}

    while(1)
    {
        std::cout << "------------------WAITNG FOR NEW CONNECTIONS-------------------" << std::endl;
		read_fds = master;
		if (select(fdMax+1, &read_fds, NULL, NULL, NULL) == -1)
			this->perror_exit("select");
		for (int i = 0; i <= fdMax; i++){
			if (FD_ISSET(i, &read_fds)){
				if (i == listener) { // handle new connection
					newfd = accept(listener,(struct sockaddr *)&remoteaddr, &addrlen);
					if (newfd == -1)
						perror("accept");
					else{
						FD_SET(newfd, &master);
						if (newfd > fdMax)
							fdMax = newfd;
				}
				} else { //handle message from client
					std::string fileStr;
					int nDataLength;
					while ((nDataLength = recv(i, buf, 200, 0)) > 0) {
						fileStr.append(buf, nDataLength);
						if (buf[nDataLength - 1] == '\n')
							break;
						bzero((void*)buf, 200);
					}
					Request request(i, fileStr);
					Answer answer(&request);
					answer.sendAnswer();
					close(i);
					FD_CLR(i, &master);
				}
			}
		}
    }
}

void    Server::perror_exit(std::string str){
    perror(str.c_str());
    exit(EXIT_FAILURE);
}