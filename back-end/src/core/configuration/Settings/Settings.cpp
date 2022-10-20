#include "Settings.hpp"

Settings::Settings()
{
	default_settings();
}

void Settings::default_settings()
{
	default_serverInfo();
}

void Settings::default_serverInfo()
{
	serverInfo.listen.push_back("0.0.0.0:80");
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
{	return (serverInfo);
}

std::vector<LocationBlock>				Settings::get_locationsList()const
{
	return (locationsList);
}

// std::map<std::string, LocationBlock>	Settings::get_pathMatch()const
// {
// 	return (pathMatch);
// }
