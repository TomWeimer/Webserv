#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../utils/Parsing/Lexer/Lexer.hpp"
#include "../Server/Server.hpp"
#include <vector>

// struct RequestLine
// {
// 	std::string				_method;
// 	std::string				_target;
// 	std::string				_version;
// };

// struct RequestHeader
// {
// 	std::string				_host;
// 	std::vector<KeyWord>	_unknown;
// };

// class Request
// {
// 	public:
// 		Server					*_server;
// 		std::string				_request;
// 		RequestLine				_request_line;
// 		RequestHeader			_request_header;
// 		std::string				_body;
// 		bool					_valid;


// 	public:
// 		Request();
// 		~Request();
// 	public:
// 	void set_server(Server* server);
// 	void set_content(std::string request);
// 	void parse_request();
// 	void check_validity();

// 	private:
// 	void parse_config_file(std::vector<KeyWord>& tokens);
		
// };

// #include "../utils/Parsing/Lexer/Lexer.hpp"
// #include "../Server/Server.hpp"
// #include <vector>

class Request
{
	private:
		class Server			*_server;
		Lexer					_http_lexer;
		BlockParams				*_actualBlock;
		bool					_valid;
		bool					_valid_target;
		bool					_valid_method;
		std::string				_full_request;
		std::vector<KeyWord>	_full_tokens;
		std::string				_method;
		std::string				_target;
		std::string				_final_target;
		std::string				_version;
		std::string				_host;
		std::vector<KeyWord>	_header_field;
		std::string				_body;
		int						_socket_fd;


	public:
		Request(int socketFD, std::string fullRequest, Server *server);
		~Request();
	private:
		bool 		assign_redirection();
		std::string	obtain_request_body();
		void		parse_request();
		void		assign_tokens();
		void 		assign_request_tokens(KeyWord* request);
		void 		assign_host_tokens(KeyWord* request);
		void 		assign_other_tokens(KeyWord* request);
		bool		check_request_validity();
		bool		minimal_http_requirement();
		void		assign_location_block();
		bool		target_is_valid();
		bool		method_is_valid();

		
	
	public:
		bool		is_valid();
		std::string	getFullRequest();
		int			getSocketFd();
		std::string getRout();
		std::string getRequestType();
};

#endif