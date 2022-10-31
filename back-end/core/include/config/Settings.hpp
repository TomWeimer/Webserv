#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include "utils/Parsing/Lexer.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#define NOT_MODIFIER 0
#define EQUAL 1
#define SERVER_BLOCK 1
#define LOCATION_BLOCK 2

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
	std::vector<int>	port;
	std::vector<std::string>	host;
	std::vector<std::string>	socket;
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
	Settings(std::vector<KeyWord> listToken);
	Settings(const Settings& origin);
	Settings& operator=(const Settings& origin);
	~Settings();

	public:

	private:
	void	default_settings();
	void	default_serverInfo();
	void	custom_settings(std::vector<KeyWord> listToken);
	void	write_settings(std::vector<KeyWord>& listToken, std::vector<KeyWord>::iterator& first);
	void	write_location_settings(KeyWord& token);
	int		find_modifier(std::string str);
	void	write_server_settings(KeyWord& token);
	void	fill_block(BlockParams&  actualBlock, KeyWord& token);
};


std::ostream& operator<<(std::ostream& out, const ServerBlock& server);
std::ostream& operator<<(std::ostream& out, const std::vector<LocationBlock> locationList);
std::ostream& operator<<(std::ostream& out, const Settings& settings);
#endif