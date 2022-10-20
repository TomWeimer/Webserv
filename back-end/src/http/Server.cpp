#include "Server.hpp"

Server::Server(){
	if ((this->_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        this->perror_exit("In socket");
	this->_PORT = 8080;
	this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons( this->_PORT );
    this->_addrlen = sizeof(this->_address);
	memset(this->_address.sin_zero, 0, sizeof this->_address.sin_zero);
	if (bind(this->_server_fd, (struct sockaddr *)&this->_address, sizeof(this->_address))<0)
        this->perror_exit("In bind");
}

Server::~Server(){
    std::cout << "server destructor called"<< std::endl;
    return;
}

void	Server::listen_connection(){
    // TODO - check how to define and handle the max number of connection
    if (listen(this->_server_fd, 10) < 0)
        this->perror_exit("In listen");
}

void	Server::handle_connection(){
    int new_socket;
    int valread = 0;

    while(1)
    {
        std::cout << "------------------WAITNG FOR NEW CONNECTIONS-------------------" << std::endl;
        if ((new_socket = accept(this->_server_fd, (struct sockaddr *)&this->_address, (socklen_t*)&this->_addrlen))<0)
            this->perror_exit("In accept");
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        Request request(new_socket, buffer);
        Answer answer(&request);
        answer.sendAnswer();
    }
}

void    Server::perror_exit(std::string str){
    perror(str.c_str());
    exit(EXIT_FAILURE);
}