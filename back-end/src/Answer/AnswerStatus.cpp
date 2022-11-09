#include "AnswerStatus.hpp"

AnswerStatus::AnswerStatus()
{
	fill_error_message();
}

AnswerStatus::~AnswerStatus() {}


std::string AnswerStatus::obtain_status_line(std::string version, int status_code)
{
	std::string status_line;

	status_line = version + " " + _status_code[status_code] + "\r\n";
	return (status_line);
}

void AnswerStatus::fill_error_message()
{
	_status_code.insert(std::make_pair(100, "100 Continue"));
	_status_code.insert(std::make_pair(101, "101 Switching Protocols"));
	_status_code.insert(std::make_pair(102, "102 Processing"));
	_status_code.insert(std::make_pair(103, "103 Early Hints"));

	_status_code.insert(std::make_pair(200, "200 OK"));
	_status_code.insert(std::make_pair(201, "201 Created"));
	_status_code.insert(std::make_pair(202, "202 Accepted"));
	_status_code.insert(std::make_pair(203, "203 Non-Authoritative Information"));
	_status_code.insert(std::make_pair(204, "204 No Content"));
	_status_code.insert(std::make_pair(205, "205 Reset Content"));
	_status_code.insert(std::make_pair(206, "206 Partial Content"));
	_status_code.insert(std::make_pair(207, "207 Multi-Status"));
	_status_code.insert(std::make_pair(208, "208 Already Reported"));
	_status_code.insert(std::make_pair(210, "210 Content Different"));
	_status_code.insert(std::make_pair(226, "226 IM Used"));

	_status_code.insert(std::make_pair(300, "300 Multiple Choices"));
	_status_code.insert(std::make_pair(301, "301 Moved Permanently"));
	_status_code.insert(std::make_pair(302, "302 Found"));
	_status_code.insert(std::make_pair(303, "303 See Other"));
	_status_code.insert(std::make_pair(304, "304 Not Modified"));
	_status_code.insert(std::make_pair(305, "305 Use Proxy"));
	_status_code.insert(std::make_pair(306, "306 Switch Proxy"));
	_status_code.insert(std::make_pair(307, "307 Temporary Redirect"));
	_status_code.insert(std::make_pair(308, "308 Permanent Redirect"));
	_status_code.insert(std::make_pair(310, "310 Too many Redirects"));

	_status_code.insert(std::make_pair(400, "400 Bad Request"));
	_status_code.insert(std::make_pair(401, "401 Unauthorized"));
	_status_code.insert(std::make_pair(402, "402 Payment Required"));
	_status_code.insert(std::make_pair(403, "403 Forbidden"));
	_status_code.insert(std::make_pair(404, "404 Not Found"));
	_status_code.insert(std::make_pair(405, "405 Method Not Allowed"));
	_status_code.insert(std::make_pair(406, "406 Not Acceptable"));
	_status_code.insert(std::make_pair(407, "407 Proxy Authentication Required"));
	_status_code.insert(std::make_pair(408, "408 Request Time-out"));
	_status_code.insert(std::make_pair(409, "409 Conflict"));
	_status_code.insert(std::make_pair(410, "410 Gone"));
	_status_code.insert(std::make_pair(411, "411 Length Required"));
	_status_code.insert(std::make_pair(412, "412 Precondition Failed"));
	_status_code.insert(std::make_pair(413, "413 Request Entity Too Large"));
	_status_code.insert(std::make_pair(414, "414 Request-URI Too Long"));
	_status_code.insert(std::make_pair(415, "415 Unsupported Media Type"));
	_status_code.insert(std::make_pair(416, "416 Requested range unsatisfiable"));
	_status_code.insert(std::make_pair(417, "417 Expectation failed"));
	_status_code.insert(std::make_pair(418, "418 I'm a teapot"));
	_status_code.insert(std::make_pair(421, "421 Bad mapping / Misdirected Request"));
	_status_code.insert(std::make_pair(422, "422 Unprocessable entity"));
	_status_code.insert(std::make_pair(423, "423 Locked"));
	_status_code.insert(std::make_pair(424, "424 Method failure"));
	_status_code.insert(std::make_pair(425, "425 Too Early"));
	_status_code.insert(std::make_pair(426, "426 Upgrade Required"));
	_status_code.insert(std::make_pair(428, "428 Precondition Required"));
	_status_code.insert(std::make_pair(429, "429 Too Many Requests"));
	_status_code.insert(std::make_pair(431, "431 Request Header Fields Too Large"));
	_status_code.insert(std::make_pair(449, "449 Retry With"));
	_status_code.insert(std::make_pair(450, "450 Blocked by Windows Parental Controls"));
	_status_code.insert(std::make_pair(451, "451 Unavailable For Legal Reasons"));
	_status_code.insert(std::make_pair(456, "456 Unrecoverable Error"));
	_status_code.insert(std::make_pair(444, "444 No Response"));
	_status_code.insert(std::make_pair(495, "495 SSL Certificate Error"));
	_status_code.insert(std::make_pair(496, "496 SSL Certificate Required"));
	_status_code.insert(std::make_pair(497, "497 HTTP Request Sent to HTTPS Port"));
	_status_code.insert(std::make_pair(498, "498 Token expired/invalid"));
	_status_code.insert(std::make_pair(499, "499 Client Closed Request"));

	_status_code.insert(std::make_pair(500, "500 Internal Server Error"));
	_status_code.insert(std::make_pair(501, "501 Not Implemented"));
	_status_code.insert(std::make_pair(502, "502 Bad Gateway ou Proxy Error"));
	_status_code.insert(std::make_pair(503, "503 Service Unavailable"));
	_status_code.insert(std::make_pair(504, "504 Gateway Time-out"));
	_status_code.insert(std::make_pair(505, "505 HTTP Version not supported"));
	_status_code.insert(std::make_pair(506, "506 Variant Also Negotiates"));
	_status_code.insert(std::make_pair(507, "507 Insufficient storage"));
	_status_code.insert(std::make_pair(508, "508 Loop detected"));
	_status_code.insert(std::make_pair(509, "509 Bandwidth Limit Exceeded"));
	_status_code.insert(std::make_pair(510, "510 Not extended"));
	_status_code.insert(std::make_pair(511, "511 Network authentication required"));
	_status_code.insert(std::make_pair(520, "520 Unknown Error"));
	_status_code.insert(std::make_pair(521, "521 Web Server Is Down"));
	_status_code.insert(std::make_pair(522, "522 Connection Timed Out"));
	_status_code.insert(std::make_pair(523, "523 Origin Is Unreachable"));
	_status_code.insert(std::make_pair(524, "524 A Timeout Occurred"));
	_status_code.insert(std::make_pair(525, "525 SSL Handshake Failed"));
	_status_code.insert(std::make_pair(526, "526 Invalid SSL Certificate"));
	_status_code.insert(std::make_pair(527, "527 Railgun Error"));
}