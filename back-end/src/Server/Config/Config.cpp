#include "Config.hpp"

// This class define the server's "settings" from a list of tokens
Config::Config(ServerBlock &info, std::vector<LocationBlock> &locations)
	: _server_params(info), _locationsList(locations), _block(NULL)
{
	create_tokens_recipes();
}

// We use a map<token_type, recipe_t> to assign the token's value to right setting.
void Config::create_tokens_recipes()
{
	new_entry("<cgi>", &Config::cgi);
	new_entry("<root>", &Config::root);
	new_entry("<host>", &Config::host);
	new_entry("<port>", &Config::port);
	new_entry("<index>", &Config::index);
	new_entry("<socket>", &Config::socket);
	new_entry("<listen>", &Config::listen);
	new_entry("<return>", &Config::redirect);
	new_entry("<rewrite>", &Config::rewrite);
	new_entry("<location>", &Config::location);
	new_entry("<autoindex>", &Config::autoindex);
	new_entry("<error_pages>", &Config::error_pages);
	new_entry("<server_name>", &Config::server_name);
	new_entry("<client_limit>", &Config::client_limit);
	new_entry("<allow_methods>", &Config::allow_methods);
}

void Config::new_entry(std::string token_name, recipe_t token_recipe)
{
	_assign.insert(std::make_pair(token_name, token_recipe));
}

void Config::server_configuration(std::vector<KeyWord> listToken)
{
	if (is_invalid(listToken))
		throw(std::runtime_error("Error with the configuration file!"));
	start_configuration(listToken);
}

bool Config::is_invalid(std::vector<KeyWord> listToken)
{
	if (listToken.empty() == true)
		return (true);
	else if (listToken[0].tokenType != "<server_start>")
		return (true);
	else if (listToken.back().tokenType != "<end>")
		return (true);
	return (false);
}

// Start setting up the server:
// - Obtain the location of the token.
// - Loop around the tokens and assign their values to the corresponding variables.
void Config::start_configuration(std::vector<KeyWord> listToken)
{
	for (std::vector<KeyWord>::iterator first = listToken.begin(); first != listToken.end(); first++)
	{
		_block = obtain_actual_block(first->tokenType);
		assign_token(*first);
	}
}

BlockParams *Config::obtain_actual_block(std::string tokenType)
{
	if (tokenType == "<server_start>" || tokenType == "<end>")
		return (&_server_params);
	else if (tokenType == "<location>")
	{
		_locationsList.push_back(LocationBlock());
		return (&_locationsList.back());
	}
	return (_block);
}


void Config::assign_token(KeyWord &keyword)
{
	key_t key;

	key = keyword.tokenType;
	if (_assign.find(key) != _assign.end())
		(this->*_assign[key])(keyword.args);
}


void Config::location(std::vector<Token> &token_content)
{
	LocationBlock *location = static_cast<LocationBlock *>(_block);

	location->modifier = token_content[0].valueToken;
	location->path = token_content[1].valueToken;
	location->cgi =  _server_params.cgi;
	location->root =  _server_params.root;
	location->index =  _server_params.index;
	location->autoindex = _server_params.autoindex;
	location->body_limit = _server_params.body_limit;
	location->error_pages =  _server_params.error_pages;
	location->redirection =  _server_params.redirection;
	location->allowed_methods = _server_params.allowed_methods;
}

// Call the functor of the listen argument, it can be a "<socket>", a "<port>" or a "<host>"
void Config::listen(std::vector<Token> &token_content)
{
	(this->*_assign[token_content[0].tokenType])(token_content);
}

// (eg: listen localhost:8080;)
void Config::socket(std::vector<Token> &token_content)
{
	std::string socket;
	size_t pos;

	socket = token_content[0].valueToken;
	pos = socket.find_first_of(':');
	assign_socket(socket.substr(0, pos), socket.substr(pos + 1));
}

// (eg: listen 0.0.0.0;)
void Config::host(std::vector<Token> &token_content)
{
	std::string host;

	host = token_content[0].valueToken;
	assign_socket(host, "80");
}

// (eg: listen 8000;)
void Config::port(std::vector<Token> &token_content)
{
	std::string port;

	port = token_content[0].valueToken;
	assign_socket("0.0.0.0", port);
}

void Config::assign_socket(std::string host, std::string port)
{
	_server_params.host.push_back(format_host(host));
	_server_params.port.push_back(StrToInt(port));
}

std::string Config::format_host(std::string host)
{
	if (host == "localhost")
		return ("127.0.0.1");
	return (host);
}

void Config::server_name(std::vector<Token> &token_content)
{
	_server_params.name.push_back(token_content[0].valueToken);
}

void Config::root(std::vector<Token> &token_content)
{
	_block->root = token_content[0].valueToken;
}

void Config::index(std::vector<Token> &token_content)
{
	_block->index.clear();
	for (std::vector<Token>::iterator it = token_content.begin(); it != token_content.end(); it++)
		_block->index.push_back(it->valueToken);
}

void Config::allow_methods(std::vector<Token> &token_content)
{
	_block->allowed_methods.clear();
	for (std::vector<Token>::iterator it = token_content.begin(); it != token_content.end(); it++)
		_block->allowed_methods.push_back(it->valueToken);
}

void Config::client_limit(std::vector<Token> &token_content)
{
	_block->body_limit = StrToInt(token_content[0].valueToken);
}

void Config::error_pages(std::vector<Token> &token_content)
{
	std::string path;
	std::vector<Token>::reverse_iterator it;

	_block->error_pages.clear();
	it = token_content.rbegin();
	path = it->valueToken;
	for (it += 1; it != token_content.rend(); it++)
	{
		int error_nb = StrToInt(it->valueToken);
		_block->error_pages.insert(std::make_pair(error_nb, path));
	}
}

void Config::redirect(std::vector<Token> &token_content)
{
	_block->redirection.type = PERMANENT;
	_block->redirection.status_code = StrToInt(token_content[0].valueToken);
	_block->redirection.text = token_content[1].valueToken;
}

void Config::rewrite(std::vector<Token> &token_content)
{
	_block->redirection.type = TEMPORARY;
	_block->redirection.text = token_content[0].valueToken;
}

void Config::autoindex(std::vector<Token> &token_content)
{
	if (token_content[0].valueToken == "on" || token_content[0].valueToken == "ON")
		_block->autoindex = ON;
	else
		_block->autoindex = OFF;
}

void Config::cgi(std::vector<Token> &token_content)
{
	_block->cgi.cgi_extension = token_content[0].valueToken;
	_block->cgi.cgi_name = token_content[1].valueToken;
}


Config::~Config(){}