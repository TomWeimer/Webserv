#include "Method.hpp"
#include "../../utils/utils.hpp"
#include "../../DirectoryListing/DirectoryListing.hpp"

Method::Method(Request* request, Response* response,int* status_code)
	: _request(request), _response(response), _status_code(status_code) {}

void Method::get()
{
	if (_request->query.size() > 0)
	{
		std::cout << "cgi allowed: " <<  _request->location->cgi.cgi_name << std::endl;
		Cgi cgi("/cgi/cgi_test.py", _request->query);
		cgi.execute_python_cgi();
		return ;
	}
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
	//  std::cerr << "body: " << _request->body  << std::endl;
	_response->body.clear();
	if (file_exists(_request->target) == true)
		set_status_code(203);
	else if(_request->chunked == true){
		handle_chunked_request();
		return ;
	}
	else
	{
		std::ofstream newFile;
		std::cerr << "target: " << _request->target.c_str() << std::endl;
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

std::string int_to_str(int nb)
{
	std::stringstream ss;
	ss << nb;
	return ss.str();
}

void Method::handle_chunked_request(){
	std::ofstream newFile;
	int	size = 0;	

	std::string body = _request->body;

	std::cout << "ici" << std::endl;
	while (body.find("\r\n")){
		std::string size = body.substr(0, body.find("\r\n"));
		int size_int = StrToHex(size);
		size += size_int;
		if (size_int == 0)
			break;
		body = body.substr(body.find("\r\n") + 2);
		std::string chunk = body.substr(0, size_int);
		body = body.substr(size_int + 2);
		newFile.open(_request->target.c_str(), std::ofstream::out | std::ofstream::app);
		newFile << chunk;
		newFile.close();
	}

	add_header("Location:", _request->target);
	add_header("Content-Length:", int_to_str(size));
	set_status_code(201);
}