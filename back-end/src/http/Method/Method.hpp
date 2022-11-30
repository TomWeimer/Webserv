#ifndef METHOD_HPP
#define METHOD_HPP
#include <iostream>
#include "../Request/Request.hpp"
#include "Cgi.hpp"
#include "../Response/Response.hpp"


class Method
{
private:
	Request		*_request;
	Response	*_response;
	int			*_status_code;
private:
	void add_header(std::string header, std::string value);
	bool need_directory_listing();
	void handle_chunked_request();
	void set_status_code(int nb);

public:
	void		get();
	void		post();
	void		delet();
public:
	Method(Request* Request, Response* response, int* status_code);
	~Method() {}
};
#endif