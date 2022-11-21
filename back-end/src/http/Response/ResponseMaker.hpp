#ifndef RESPONSEMAKER_HPP
#define RESPONSEMAKER_HPP

#include "Response.hpp"
#include "../../utils/utils.hpp"
#include "../../Server/Settings/Settings.hpp"
#include "../Request/Request.hpp"

class ResponseMaker
{
private:
	int*						_status_code;
	Request*					_request;
	Response*					_response;
	std::map<int, std::string> 	_status_message;

public:
	ResponseMaker(Response *response, Request *request, int* status_code);
	~ResponseMaker(){}

public:
	void	make_response();

private:
	int				status_code();
	void			set_status_code(int nb);
	void			fill_error_message();
	std::string		method();
	std::string		redirection();
	std::string		error_page();
	std::string		redirection_or_error_page();
	void			add_header(std::string new_header, std::string value_header);
	std::string		new_error_page(int code);
	std::string		new_error_page(std::string message);
};
#endif
