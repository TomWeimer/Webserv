#include "tests.hpp"
#include "config/Settings.hpp"

bool testSettings()
{
	bool resultTests;

	resultTests = default_Settings();
	std::cerr << "testSettings: ";
	if (resultTests == true)
		std::cerr << GREEN << "ok" << std::endl;
	else
		std::cerr << RED << "ko" << std::endl;
	return (resultTests);
}

bool default_Settings()
{
	Settings settings_default;
	ServerBlock testInfo = settings_default.get_serverInfo();

	if (testInfo.port[0] != 80)
		return (false);
	if (testInfo.host[0] != "0.0.0.0")
		return (false);
	if (testInfo.socket[0] != "0.0.0.0:80")
		return (false);
	if (testInfo.name.empty() != true)
		return (false);
	if (testInfo.root.empty() != true)
		return (false);
	if (testInfo.body_limit != 0)
		return (false);
	if (testInfo.allowed_methods.size() != 3)
		return (false);
	if (testInfo.allowed_methods[0] != "GET")
		return (false);
	if (testInfo.allowed_methods[1] != "POST")
		return (false);
	if (testInfo.allowed_methods[2] != "DELETE")
		return (false);

	return (true);
}