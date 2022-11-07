#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP
#include "Socket.hpp"

class IServerSocket
{
	virtual void	socket()                 = 0;
	virtual void	bind()                   = 0;
	virtual void	listen(int queue)        = 0;
	virtual int		accept()                 = 0;
	virtual void	close()                  = 0;
	public:
	virtual  ~IServerSocket() {}
};



class ServerSocket : public IServerSocket, public Socket
{
	public:
	void	socket();
	void	bind();
	void	listen(int queue);
	int		accept();
	void	close();

	public:
	int sockfd() const;
	struct sockaddr_in& get_name();
	const struct sockaddr_in& get_name() const;
	
	
	public:
	ServerSocket(int family, int port, std::string ip);
	ServerSocket(const ServerSocket&);
	ServerSocket& operator=(const ServerSocket&);
	virtual ~ServerSocket();

	public:
	bool is_listening_port()const;
};

#endif