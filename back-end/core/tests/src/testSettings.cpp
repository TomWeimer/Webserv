#include "tests.hpp"
#include "config/Settings.hpp"


bool testSettings()
{
//	std::cerr << Settings();
	execute_test("default settings: ", &default_Settings);
	execute_test("custom settings: ", &custom_Settings);

	return (true);
}

bool default_Settings()
{
	std::stringstream	ssr;
	Settings			settings_default;
	Logfile				log("./back-end/core/tests/logs/result/default_settings.log");

	log.clear();
	ssr << settings_default;
	log << ssr;
	return (compare_file("./back-end/core/tests/logs/result/default_settings.log", "./back-end/core/tests/logs/expected/default_settings.log"));
}

bool custom_Settings()
{
	Lexer				lex("./back-end/core/tests/conf/rules1.ebnf", "./back-end/core/tests/conf/configFile1.conf");
	std::stringstream	ssr;
	Settings			settings_default(lex.lexeme());
	Logfile				log("./back-end/core/tests/logs/result/custom_settings.log");

	log.clear();
	ssr << settings_default;
	log << ssr;
	return (compare_file("./back-end/core/tests/logs/result/custom_settings.log", "./back-end/core/tests/logs/expected/custom_settings.log"));
}

// bool custom_Settings()
// {
// 	Lexer		myLexer("./back-end/core/tests/conf/")
// 	Settings	custom_setting(myLexer.lexeme());
// 	ServerBlock testInfo = settings_default.get_serverInfo();

// 	if (testInfo.port[0] != 80)
// 		return (false);
// 	if (testInfo.host[0] != "0.0.0.0")
// 		return (false);
// 	if (testInfo.socket[0] != "0.0.0.0:80")
// 		return (false);
// 	if (testInfo.name.empty() != true)
// 		return (false);
// 	if (testInfo.root.empty() != true)
// 		return (false);
// 	if (testInfo.body_limit != 0)
// 		return (false);
// 	if (testInfo.allowed_methods.size() != 3)
// 		return (false);
// 	if (testInfo.allowed_methods[0] != "GET")
// 		return (false);
// 	if (testInfo.allowed_methods[1] != "POST")
// 		return (false);
// 	if (testInfo.allowed_methods[2] != "DELETE")
// 		return (false);

// 	return (true);
// }