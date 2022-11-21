#include "Method.hpp"
#include "../../utils/utils.hpp"
#include "../../DirectoryListing/DirectoryListing.hpp"

Method::Method(Request* request, Response* response,int* status_code)
	: _request(request), _response(response), _status_code(status_code) {}

void Method::get()
{
	_response->body.clear();
	if (need_directory_listing() == true)
	{
		DirectoryListing	listing;
	
		_response->body = listing.obtain_directory_content(_request->target, _request->location->root);
	}
	else
		_response->body = obtain_body_content(_request->target);
	add_header("Content-Length:", NumberToString(_response->body.size()));
	set_status_code(200);
}

bool Method::need_directory_listing()
{
	return (_request->location->autoindex == ON && isDir(_request->target) == true);
}

void Method::post()
{
	std::cerr << "body: " << _request->body  << std::endl;
	_response->body.clear();
	if (file_exists(_request->target) == true)
		set_status_code(203);
	else
	{
		std::ofstream newFile;
		newFile.open(_request->target.c_str(), std::ofstream::out); // for cgi appending: ofs.open ("test.txt", std::ofstream::out | std::ofstream::app);
		newFile << _request->body;
		newFile.close();
		add_header("Location:", _request->target);
		set_status_code(201);
	}
	add_header("Content-Length:", NumberToString(_response->body.size()));
}

void Method::delet()
{
	if (file_exists(_request->target) == false)
		set_status_code(204);
	else
	{
		if (std::remove(_request->target.c_str()))
		{
			_response->body = "<html>\n  <body>\n    <h1>File deleted.</h1>\n  </body>\n</html>";
			set_status_code(200);
		}
		else
			set_status_code(202); // 202 Accepted
	}
	add_header("Content-Length:", NumberToString(_response->body.size()));
}

void	Method::add_header(std::string header, std::string value)
{
	_response->add_header(header, value);
}

void Method::set_status_code(int nb)
{
	if ((*_status_code) == 0)
		*_status_code = nb;
}