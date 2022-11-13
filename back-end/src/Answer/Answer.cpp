#include "Answer.hpp"

std::string readContent(int fd)
{
    std::string ret;
    char buf[4096 + 1];
    int readed;

    lseek(fd, 0, SEEK_SET);
    while ((readed = read(fd, buf, 4096)) != 0)
    {
        if (readed == -1)
            return "";
        buf[readed] = '\0';
        ret.insert(ret.length(), buf, readed);
    }
    return ret;
}

Answer::Answer(Server *server, Request *request)
{
	_server = server;
	_request = request;
}

Answer::~Answer(){}

std::string Answer::message()
{	
	return (create_message());
}

std::string Answer::create_message()
{
	AnswerBody		_body(_server, _request);
	AnswerStatus	_status;
	AnswerHeader	_header;

	std::string tmp;
	std::string rtn;

	if (_request->_target == "/cgi")
	{
		if (_request->_method == "GET")
			cgi_get_request();
	}

	tmp = _body.obtain_body(&_header);
	rtn = _status.obtain_status_line(_request->_version, _server->get_status_code());
	rtn += _header.obtain_headers();
	rtn += "\r\n";
	rtn += "\r\n";
	rtn += tmp;

	
	return (rtn);

}

void	Answer::set_cgi_env(std::string method){
		char **env = new char*[18];
		env[0] = strdup("SERVER_NAME=127.0.0.1");
		env[1] = strdup("CONTENT_TYPE=text/html; charset=utf-8");
		if (method == "POST")
			env[2] = strdup("REQUEST_METHOD=POST");
		else if (method == "DELETE")
			env[2] = strdup("REQUEST_METHOD=DELETE");
		else
			env[2] = strdup("REQUEST_METHOD=GET");
		env[3] = strdup("GATEWAY_INTERFACE=CGI/1.1");
		env[4] = strdup("FILE_UPLOADS=On");
		env[5] = strdup("QUERY_STRING=");
		env[6] = strdup("PATH_INFO=/Users/yacinebentayeb/Desktop/Webserv/front-end/html/test.html");
		env[7] = strdup("PATH_TRANSLATED=/Users/yacinebentayeb/Desktop/Webserv/front-end/html/test.html");
		env[8] = strdup("REMOTE_HOST=");
		env[9] = strdup("REQUEST_URI=/Users/yacinebentayeb/Desktop/Webserv/front-end/html/test.html");
		env[10] = strdup("REDIRECT_STATUS=0");
		if (method == "POST")
		{
			std::string content_len = "CONTENT_LENGTH=" + std::to_string(_request->_body.size());
			env[11] = strdup(content_len.c_str());
		}
		else
			env[11] = strdup("CONTENT_LENGTH=0");
		env[12] = strdup("SERVER_PROTOCOL=HTTP/1.1");
		env[13] = strdup("SERVER_PORT=4242");
		env[14] = strdup("SERVER_SOFTWARE=WetServ/1.0");
		env[15] = strdup("SCRIPT_NAME=/Users/yacinebentayeb/Desktop/Webserv/cgi/cgi_server");
		env[16] = 0;
		_cgi_env = env;
}


void	Answer::cgi_get_request(){
	char **argv = new char*[3];
	argv[0] = strdup("/Users/yacinebentayeb/Desktop/Webserv/cgi/cgi_server");
	argv[1] = strdup("/Users/yacinebentayeb/Desktop/Webserv/front-end/html/index.html");
	argv[2] = 0;
	set_cgi_env("GET");
	int pip_to_cgi[2];
	int pip_from_cgi[2];

	if (pipe(pip_to_cgi) != 0 || pipe(pip_from_cgi) != 0)
		perror("pipe");

	int pid = fork();
	if (pid == 0)
	{
		if (dup2(pip_to_cgi[0], STDIN_FILENO) == -1)
			perror("Can't duplicate pip_to_cgi to STDIN_FILENO");
		if (dup2(pip_from_cgi[1], STDOUT_FILENO) == -1)
			perror("Can't duplicate pip_from_cgi to STDOUT_FILENO");
		close(pip_from_cgi[0]);
		close(pip_from_cgi[1]);
		close(pip_to_cgi[0]);
		close(pip_to_cgi[1]);
		if (execve("/Users/yacinebentayeb/Desktop/Webserv/cgi/cgi_tester", argv, _cgi_env) == -1)
			perror("execve");
	}
	else
	{
		int status;
		close(pip_to_cgi[0]);
		close(pip_from_cgi[1]);
		// for post request
		// char *buf = new char[10000];
		// write(pip_to_cgi[1], buf, 100);
		// std::cerr << buf << std::endl;
		close(pip_to_cgi[1]);
		pip_to_cgi[1] = -1;
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
		std::string ret = readContent(pip_from_cgi[0]);
		std::cout << ret << std::endl;
		close(pip_from_cgi[0]);
	}
}


