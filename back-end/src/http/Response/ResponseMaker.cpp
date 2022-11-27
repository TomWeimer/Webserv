#include "ResponseMaker.hpp"
#include "../Method/Method.hpp"

ResponseMaker::ResponseMaker(Response* response, Request* request, int* status_code)
	: _status_code(status_code), _request(request), _response(response)
{
	fill_error_message();
}

void ResponseMaker::make_response()
{
	if (status_code() == 0)
		_response->body = method();
	else
		_response->body = redirection_or_error_page();
	_response->status = _status_message[status_code()];
	_response->version = _request->version;
	// std::cerr << "response: " << _response->make_response() << std::endl;
	_response->make_response();
}

std::string ResponseMaker::method()
{
	Method method(_request, _response, _status_code);

	if (_request->method == "GET")
		method.get();
	else if (_request->method == "POST")
		method.post();
	else if (_request->method == "DELETE")
		method.delet();	
	return _response->body;
}

std::string ResponseMaker::redirection_or_error_page()
{
	if (_request->redirected == PERMANENT || _request->redirected == TEMPORARY)
		return (redirection());
	else
		return (error_page());

}

std::string  ResponseMaker::redirection()
{
	if (_request->redirected == PERMANENT && status_code() != 301)
		return (new_error_page(_request->location->redirection.text));
	else
	{
		_response->add_header("Location:", _request->location->redirection.text);
		return ("");
	}
}

std::string ResponseMaker::error_page()
{
	std::map<int, std::string>* error_page;

	error_page = &_request->location->error_pages;
	if (error_page->find(status_code()) == error_page->end())
		return (new_error_page(status_code()));
	else
		return (obtain_body_content((*error_page)[status_code()]));
}

std::string ResponseMaker::new_error_page(int code)
{
	std::string message;

	message = _status_message[code];
	return (new_error_page(message));
}

std::string ResponseMaker::new_error_page(std::string message)
{
	std::string html;

	html += "<!DOCTYPE html>\n";
	html += "<html lang=\"en\">\n";
	html += "<head>\n";
	html += "<meta charset=\"UTF-8\">\n";
	html += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
	html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	html += "<title>ErrorPage</title>\n";
	html += "</head>\n";
	html += "<body>\n";
	html += "<h1>Error Page</h1>\n";
	html += "<span>" + message + "</span>\n";
	html += "</body>\n";
	html += "</html>\n";
	return (html);
}


int ResponseMaker::status_code()
{
	return (*_status_code);
}

void ResponseMaker::set_status_code(int nb)
{
	if ((*_status_code) == 0)
		*_status_code = nb;
}

void ResponseMaker::fill_error_message()
{
	_status_message.insert(std::make_pair(100, "100 Continue"));
	_status_message.insert(std::make_pair(101, "101 Switching Protocols"));
	_status_message.insert(std::make_pair(102, "102 Processing"));
	_status_message.insert(std::make_pair(103, "103 Early Hints"));
	_status_message.insert(std::make_pair(200, "200 OK"));
	_status_message.insert(std::make_pair(201, "201 Created"));
	_status_message.insert(std::make_pair(202, "202 Accepted"));
	_status_message.insert(std::make_pair(203, "203 Non-Authoritative Information"));
	_status_message.insert(std::make_pair(204, "204 No Content"));
	_status_message.insert(std::make_pair(205, "205 Reset Content"));
	_status_message.insert(std::make_pair(206, "206 Partial Content"));
	_status_message.insert(std::make_pair(207, "207 Multi-Status"));
	_status_message.insert(std::make_pair(208, "208 Already Reported"));
	_status_message.insert(std::make_pair(210, "210 Content Different"));
	_status_message.insert(std::make_pair(226, "226 IM Used"));
	_status_message.insert(std::make_pair(300, "300 Multiple Choices"));
	_status_message.insert(std::make_pair(301, "301 Moved Permanently"));
	_status_message.insert(std::make_pair(302, "302 Found"));
	_status_message.insert(std::make_pair(303, "303 See Other"));
	_status_message.insert(std::make_pair(304, "304 Not Modified"));
	_status_message.insert(std::make_pair(305, "305 Use Proxy"));
	_status_message.insert(std::make_pair(306, "306 Switch Proxy"));
	_status_message.insert(std::make_pair(307, "307 Temporary Redirect"));
	_status_message.insert(std::make_pair(308, "308 Permanent Redirect"));
	_status_message.insert(std::make_pair(310, "310 Too many Redirects"));
	_status_message.insert(std::make_pair(400, "400 Bad Request"));
	_status_message.insert(std::make_pair(401, "401 Unauthorized"));
	_status_message.insert(std::make_pair(402, "402 Payment Required"));
	_status_message.insert(std::make_pair(403, "403 Forbidden"));
	_status_message.insert(std::make_pair(404, "404 Not Found"));
	_status_message.insert(std::make_pair(405, "405 Method Not Allowed"));
	_status_message.insert(std::make_pair(406, "406 Not Acceptable"));
	_status_message.insert(std::make_pair(407, "407 Proxy Authentication Required"));
	_status_message.insert(std::make_pair(408, "408 Request Time-out"));
	_status_message.insert(std::make_pair(409, "409 Conflict"));
	_status_message.insert(std::make_pair(410, "410 Gone"));
	_status_message.insert(std::make_pair(411, "411 Length Required"));
	_status_message.insert(std::make_pair(412, "412 Precondition Failed"));
	_status_message.insert(std::make_pair(413, "413 Request Entity Too Large"));
	_status_message.insert(std::make_pair(414, "414 Request-URI Too Long"));
	_status_message.insert(std::make_pair(415, "415 Unsupported Media Type"));
	_status_message.insert(std::make_pair(416, "416 Requested range unsatisfiable"));
	_status_message.insert(std::make_pair(417, "417 Expectation failed"));
	_status_message.insert(std::make_pair(418, "418 I'm a teapot"));
	_status_message.insert(std::make_pair(421, "421 Bad mapping / Misdirected Request"));
	_status_message.insert(std::make_pair(422, "422 Unprocessable entity"));
	_status_message.insert(std::make_pair(423, "423 Locked"));
	_status_message.insert(std::make_pair(424, "424 Method failure"));
	_status_message.insert(std::make_pair(425, "425 Too Early"));
	_status_message.insert(std::make_pair(426, "426 Upgrade Required"));
	_status_message.insert(std::make_pair(428, "428 Precondition Required"));
	_status_message.insert(std::make_pair(429, "429 Too Many Requests"));
	_status_message.insert(std::make_pair(431, "431 Request Header Fields Too Large"));
	_status_message.insert(std::make_pair(449, "449 Retry With"));
	_status_message.insert(std::make_pair(450, "450 Blocked by Windows Parental Controls"));
	_status_message.insert(std::make_pair(451, "451 Unavailable For Legal Reasons"));
	_status_message.insert(std::make_pair(456, "456 Unrecoverable Error"));
	_status_message.insert(std::make_pair(444, "444 No Response"));
	_status_message.insert(std::make_pair(495, "495 SSL Certificate Error"));
	_status_message.insert(std::make_pair(496, "496 SSL Certificate Required"));
	_status_message.insert(std::make_pair(497, "497 HTTP Request Sent to HTTPS Port"));
	_status_message.insert(std::make_pair(498, "498 Token expired/invalid"));
	_status_message.insert(std::make_pair(499, "499 Client Closed Request"));
	_status_message.insert(std::make_pair(500, "500 Internal Server Error"));
	_status_message.insert(std::make_pair(501, "501 Not Implemented"));
	_status_message.insert(std::make_pair(502, "502 Bad Gateway ou Proxy Error"));
	_status_message.insert(std::make_pair(503, "503 Service Unavailable"));
	_status_message.insert(std::make_pair(504, "504 Gateway Time-out"));
	_status_message.insert(std::make_pair(505, "505 HTTP Version not supported"));
	_status_message.insert(std::make_pair(506, "506 Variant Also Negotiates"));
	_status_message.insert(std::make_pair(507, "507 Insufficient storage"));
	_status_message.insert(std::make_pair(508, "508 Loop detected"));
	_status_message.insert(std::make_pair(509, "509 Bandwidth Limit Exceeded"));
	_status_message.insert(std::make_pair(510, "510 Not extended"));
	_status_message.insert(std::make_pair(511, "511 Network authentication required"));
	_status_message.insert(std::make_pair(520, "520 Unknown Error"));
	_status_message.insert(std::make_pair(521, "521 Web Server Is Down"));
	_status_message.insert(std::make_pair(522, "522 Connection Timed Out"));
	_status_message.insert(std::make_pair(523, "523 Origin Is Unreachable"));
	_status_message.insert(std::make_pair(524, "524 A Timeout Occurred"));
	_status_message.insert(std::make_pair(525, "525 SSL Handshake Failed"));
	_status_message.insert(std::make_pair(526, "526 Invalid SSL Certificate"));
	_status_message.insert(std::make_pair(527, "527 Railgun Error"));
}