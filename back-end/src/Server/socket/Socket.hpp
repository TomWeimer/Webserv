#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

class Socket {
	private:
		int 				_socketFd;
		struct sockaddr_in	_address;
		int					_addrlen;
		bool				_isListening;
		int					_PORT;
		char				*_requestBuffer;


	public:
		Socket(bool isListening, int port); 	//constructor for server/listening socket
		Socket(Socket * ListeningSocket, bool isListening); 	//constructor for client socket
		~Socket();

		
		int		getSocketFd();
		int		getAddrlen();
		int		*getPtrAddrlen(); //needed the for accept function
		int		getPort();
		struct	sockaddr_in		getAddress();
		struct	sockaddr_in 	*getPtrAddress(); //needed for the accept function

		void	bindSocket();
		void	listenPort(int queueLen);
		void	perrorExit(std::string str);
		void	receiveRequest();

};
