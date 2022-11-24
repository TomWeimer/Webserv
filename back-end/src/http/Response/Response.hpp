#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <map>

struct Response
{
public:
	std::string status;
	std::string version;
	std::map<std::string, std::string> header;
	std::string body;

public:
	void add_header(std::string header, std::string value)
	{
		this->header.insert(std::make_pair(header, value));
	}
	std::string make_response()
	{
		std::string response;
		std::map<std::string, std::string>::iterator it;
		response += version + " " + status + "\r\n";
		for (it = header.begin(); it != header.end(); it++)
		{
			response += it->first + " " + it->second + "\r\n";
		}
		response += "\r\n";
		response += body;
		return (response);
	}
};
#endif