#ifndef SERVER_HPP
#define SERVER_HPP
#include "../Config/Config.hpp"
#include "../Socket/ServerSocket.hpp"
#include "../Answer/AnswerHeader.hpp"

class Server
{
private:
	
	ServerBlock						_info;
	std::vector<LocationBlock>		_locationsList;
	std::vector<ServerSocket>		_server_sockets;
	int								_status_code;
	

public:
	Server(std::vector<KeyWord> tokens);
	~Server();

private:
	void init_server_sockets();
	void serverConfiguration(std::vector<KeyWord> tokens);

public:
	int				get_status_code()const;
	void			set_status_code(int number);
	int 			get_port(int i);
	ServerSocket 	&getSocket(int i);
	ServerBlock		*get_server_info();
	BlockParams		*find_location(std::string target);
	std::string		obtain_final_target(BlockParams *_location, std::string _target);
	bool			is_valid_target(std::string _target);
	bool			is_valid_method(std::string method, BlockParams *location);
	bool			no_error();

	void 			process_post(AnswerHeader* header_list, std::string& body, std::string target);
	bool			post_check_file_already_exist(std::string target);
	void 			process_get(AnswerHeader* header_list, std::string& body, std::string target);
	void			process_delete(AnswerHeader* header_list, std::string& body, std::string target);
	bool			delete_check_if_file_exist(std::string target);

	public:
	void reset_status_code();
	std::vector<LocationBlock>& get_location_list();
	ServerSocket& operator[](int index);
	size_t size();
	private:
	std::string obtain_body_content(std::string target);
};

std::string NumberToString(size_t nb);

#endif
