#include "config/Settings.hpp"


Settings::Settings()
{
	default_settings();
}

Settings::Settings(std::vector<KeyWord> listToken)
{
	custom_settings(listToken);
}

void Settings::default_settings()
{
	default_serverInfo();
}

void Settings::default_serverInfo()
{
	serverInfo.socket.push_back("0.0.0.0:80");
	serverInfo.port.push_back(8080);
	serverInfo.port.push_back(8081);
	serverInfo.port.push_back(8082);
	serverInfo.host.push_back("0.0.0.0");
	//serverInfo.name.insert("");
	serverInfo.root = "";
	//serverInfo.index = ""; ???
	//serverInfo.error_pages = ""; ???
	serverInfo.body_limit = 0;
	serverInfo.allowed_methods.push_back("GET");
	serverInfo.allowed_methods.push_back("POST");
	serverInfo.allowed_methods.push_back("DELETE");
}

Settings::Settings(const Settings& origin)
	: serverInfo(origin.serverInfo), locationsList(origin.locationsList){}

Settings& Settings::operator=(const Settings& origin)
{
	if (this == &origin)
		return (*this);
	serverInfo = origin.serverInfo;
	locationsList = origin.locationsList;
	// pathMatch = origin.pathMatch;
	return (*this);
}

Settings::~Settings() {}

ServerBlock								Settings::get_serverInfo()const
{
	return (serverInfo);
}

std::vector<LocationBlock>				Settings::get_locationsList()const
{
	return (locationsList);
}


void Settings::custom_settings(std::vector<KeyWord> listToken)
{
	std::vector<KeyWord>::iterator first;
	std::vector<KeyWord>::iterator last;

	first = listToken.begin();
	last = listToken.end();

	if (first->tokenType == "<server_start>")
	{
		for(; first != last; first++)
		{
			if (first->tokenType == "<server_start>")
			{
				serverInfo.body_limit = 0;
				serverInfo.root = "";
			}
			else
				write_settings(listToken, first);
		}
	}
	
}

void Settings::write_settings(std::vector<KeyWord>& listToken, std::vector<KeyWord>::iterator& first)
{
	static int actual_block = SERVER_BLOCK;

	if (first->tokenType == "<location>")
		actual_block = LOCATION_BLOCK;
	else if (actual_block == LOCATION_BLOCK && first->tokenType == "<end>")
		actual_block = SERVER_BLOCK;
	else if (listToken.empty() == true)
		actual_block = SERVER_BLOCK;
	if (actual_block == LOCATION_BLOCK)
		write_location_settings(*first);
	else
	{
		write_server_settings(*first);
	}
}

void	Settings::write_location_settings(KeyWord& token)
{
	LocationBlock	*actualLocation;

	
	
	if (token.tokenType == "<location>")
		locationsList.push_back(LocationBlock());
	actualLocation = &locationsList.back();

	if (token.tokenType == "<location>")
	{
		actualLocation->modifier = find_modifier(token.args[0].valueToken);
		actualLocation->path = token.args[1].valueToken;
		
		actualLocation->body_limit = 0;
		actualLocation->root = "";
	}
	else
		fill_block(*actualLocation, token);
}

int		Settings::find_modifier(std::string str)
{
	if (str == "=")
	{
		return (EQUAL);
	}
	return (NOT_MODIFIER);
}

void	Settings::write_server_settings(KeyWord& token)
{
	std::string tmp;
	size_t		pos;
	
	if (token.tokenType == "<listen>")
	{

		if (token.args[0].tokenType == "<socket>")
		{
			serverInfo.socket.push_back(token.args[0].valueToken);

			pos = token.args[0].valueToken.find_first_of(':');
			tmp = token.args[0].valueToken.substr(0, pos + 1);
			serverInfo.port.push_back((int)std::strtol(tmp.c_str(), NULL, 10));

			tmp = token.args[0].valueToken.substr(pos + 1, token.args[0].valueToken.size() - (pos + 1));
			serverInfo.host.push_back(tmp);
		}
		else if (token.args[0].tokenType == "<host>")
		{
			tmp = token.args[0].valueToken;
			serverInfo.host.push_back(tmp);
			serverInfo.port.push_back(80);
			tmp += ":80";
			serverInfo.socket.push_back(tmp);
		}
		else if (token.args[0].tokenType == "<port>")
		{

			tmp = token.args[0].valueToken;  
			serverInfo.port.push_back((int)std::strtol(tmp.c_str(), NULL, 10));
			serverInfo.host.push_back( "0.0.0.0");
			tmp.insert(0, "0.0.0.0:");
			serverInfo.socket.push_back(tmp);
		}
	}
	else if (token.tokenType == "<server_name>")
	{
		serverInfo.name.push_back(token.args[0].valueToken);
	}
	else
		fill_block(serverInfo, token);
}

void Settings::fill_block(BlockParams&  actualBlock, KeyWord& token)
{
	std::string			tmp;
	char				*end;
	int					error_nb;

	if (token.tokenType == "<root>")
	{
		actualBlock.root = token.args[0].valueToken;
	}
	else if (token.tokenType == "<index>")
	{
		std::stringstream ssr(token.args[0].valueToken);
		while(ssr.eof() == false)
		{
			ssr >> tmp;
			
			actualBlock.index.push_back(tmp);
		}
	}
	else if (token.tokenType == "<allowed_methods>")
	{
		std::stringstream ssr(token.args[0].valueToken);
		while(ssr.eof() == false)
		{
			ssr >> tmp;
			actualBlock.allowed_methods.push_back(tmp);
		}
	}
	else if (token.tokenType == "<client_limit>")
	{
		tmp = token.args[0].valueToken;
		actualBlock.body_limit = (int)std::strtol(tmp.c_str(), NULL, 10);
	}
	else if (token.tokenType == "<error_pages>")
	{
		std::string path;
		path = token.args[1].valueToken;
		tmp = token.args[0].valueToken;
		while (tmp.empty() == false)
		{
			error_nb = (int)std::strtol(tmp.c_str(), &end, 10);
			tmp = end;
			actualBlock.error_pages.insert(std::make_pair(error_nb, path));
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Settings& settings)
{
	std::vector<LocationBlock> locationBlock;

	out << settings.get_serverInfo();
	locationBlock = settings.get_locationsList();
	if (locationBlock.empty() == false)
		out << settings.get_locationsList();
	return (out);
}

std::ostream& operator<<(std::ostream& out, const ServerBlock& serverInfo)
{
	out << "Server content:" << std::endl;
	out << "	name: ";
	for (std::vector<std::string>::const_iterator it = serverInfo.name.begin(); it != serverInfo.name.end(); it++)
	{
		out << *it;
		if (it + 1 != serverInfo.name.end())
			out << ", ";
	}
	out << std::endl;
	size_t index = 0;
	for (std::vector<std::string>::const_iterator it = serverInfo.socket.begin(); it != serverInfo.socket.end(); it++)
	{
		out << "	port: " << serverInfo.port[index] << ", host: " << serverInfo.host[index] << ", socket: " << *it << std::endl; 
		index++;
	}

	out << "	root: " << serverInfo.root << std::endl;
	out << "	allowed_methods: ";
	for (std::vector<std::string>::const_iterator it = serverInfo.allowed_methods.begin(); it != serverInfo.allowed_methods.end(); it++)
	{
		out << *it;
		if (it + 1 != serverInfo.allowed_methods.end())
			out << ", ";
	}
	out << std::endl;
	out<< "	index: ";
	for (std::vector<std::string>::const_iterator it = serverInfo.index.begin(); it != serverInfo.index.end(); it++)
	{
		out << *it;
		if (it + 1 != serverInfo.index.end())
			out << ", ";
	}
	out << std::endl;
	out << "\n	error_pages: " << std::endl;
	for (std::map<int, std::string>::const_iterator it = serverInfo.error_pages.begin(); it != serverInfo.error_pages.end(); it++)
	{
		out << "	" << it->first << " -> " << it->second << std::endl;
	}
	out << "	body limit: " << serverInfo.body_limit << std::endl;
	return (out);
}

std::ostream& operator<<( std::ostream& out, const std::vector<LocationBlock> locationList)
{
	for (std::vector<LocationBlock>::const_iterator it2 = locationList.begin(); it2 != locationList.end(); it2++)
	{
		out << "\n	location";
		if (it2->modifier == EQUAL)
			out << " = ";
		out  << it2->path << "\n	{" << std::endl;
		

		out << "		root: " << it2->root << std::endl;
	out << "		allowed_methods: ";
	for (std::vector<std::string>::const_iterator it = it2->allowed_methods.begin(); it != it2->allowed_methods.end(); it++)
	{
		out << *it;
		if (it + 1 != it2->allowed_methods.end())
			out << ", ";
	}
	out << std::endl;
	out<< "		index: ";
	for (std::vector<std::string>::const_iterator it = it2->index.begin(); it != it2->index.end(); it++)
	{
		out << *it;
		if (it + 1 != it2->index.end())
			out << ", ";
	}
	out << std::endl;
	out << "		error_pages: " << std::endl;
	for (std::map<int, std::string>::const_iterator it = it2->error_pages.begin(); it != it2->error_pages.end(); it++)
	{
		out << it->first << " -> " << it->second << std::endl;
	}
	out << "		body limit: " << it2->body_limit << std::endl;
	out << "	}" << std::endl;
	}
	return (out);
}