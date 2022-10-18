#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <iostream>
#include <vector>
#include<map>

struct BlockParams
{
	int 						body_limit;
	std::string 				root;
	std::vector<std::string> 	index;
	std::map<int, std::string>	error_pages;
	std::vector<std::string>	allowed_methods;
};

struct LocationBlock : public BlockParams
{
	std::string path;
	int			modifier;
};


struct ServerBlock : public BlockParams
{
	std::vector<std::string>	listen;
	std::vector<std::string>	name;
};

class Settings
{
	private:
	//---------------------------HERE-----------------------------
	ServerBlock 							serverInfo;
	std::vector<LocationBlock>				locationsList;
	//std::map<std::string, LocationBlock>	pathMatch;
	//---------------------------END------------------------------

	public:
	ServerBlock								get_serverInfo()const;
	std::vector<LocationBlock>				get_locationsList()const;
	std::map<std::string, BlockParams *>	get_pathMatch()const;

	public:
	Settings();
	//Settings(std::vector<Token *> listToken);
	Settings(const Settings& origin);
	Settings& operator=(const Settings& origin);
	~Settings();

	public:

	private:
	void	default_settings();
	void	default_serverInfo();
};

#endif