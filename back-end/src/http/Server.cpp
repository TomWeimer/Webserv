#include "Server.hpp"

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

    int valread = 0;

    while(1)
    {
        std::cout << "------------------WAITNG FOR NEW CONNECTIONS-------------------" << std::endl;
        Socket clientSocket(&this->_listeningSockets[0], false);
        char buffer[30000] = {0};
        valread = read( clientSocket.getSocketFd() , buffer, 30000);
        Request request(clientSocket.getSocketFd(), buffer);
        Answer answer(&request);
        answer.sendAnswer();
    }
}

void    Server::perror_exit(std::string str){
    perror(str.c_str());
    exit(EXIT_FAILURE);
}