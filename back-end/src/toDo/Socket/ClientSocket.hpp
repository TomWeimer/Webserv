#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP
#include "Socket.hpp"

class IClientSocket
{
	virtual std::string	recv(int buffer_size)				= 0;
	virtual void		send(std::string response_message)	= 0;
	virtual void		close()                				= 0;
	public:
	virtual  ~IClientSocket() {}
};

class ClientSocket : public IClientSocket, public Socket
{
	public:
	std::string	recv(int buffer_size);
	void		send(std::string response_message);
	void		close();

	public:
	int							sockfd() const;
	struct sockaddr_in&			get_name();
	const struct sockaddr_in&	get_name() const;
	
	
	public:
	ClientSocket(int sockfd);
	ClientSocket(const ClientSocket&);
	ClientSocket& operator=(const ClientSocket&);
	virtual ~ClientSocket();

	public:
	bool is_listening_port()const;
};

#endif