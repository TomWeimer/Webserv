#include "Answer.hpp"

std::string read_answer(int fd)
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
		execute_cgi_request();
	}

	tmp = _body.obtain_body(&_header);
	rtn = _status.obtain_status_line(_request->_version, _server->get_status_code());
	rtn += _header.obtain_headers();
	rtn += "\r\n";
	rtn += "\r\n";
	rtn += tmp;

	
	return (rtn);

}

void	Answer::set_cgi_env(){ // set the environment variables for the cgi
		char **env = new char*[18];
		char *path = new char[2000];
		std::string request_mtd = "REQUEST_METHOD=" + _request->_method;
		if (!getwd(path))
			perror("getwd");
		std::string path_file = path;
		path_file += "/front-end/html/test.html";

		env[0] = strdup("SERVER_NAME=127.0.0.1");
		env[1] = strdup("CONTENT_TYPE=text/html; charset=utf-8");
		env[2] = strdup((char *)request_mtd.c_str());
		env[3] = strdup("GATEWAY_INTERFACE=CGI/1.1");
		env[4] = strdup("FILE_UPLOADS=On");
		env[5] = strdup("QUERY_STRING=");
		env[6] = strdup(("PATH_INFO=" + path_file).c_str());
		env[7] = strdup(("PATH_TRANSLATED=" + path_file).c_str());
		env[8] = strdup("REMOTE_HOST=");
		env[9] = strdup(("REQUEST_URI=" + path_file).c_str());
		env[10] = strdup("REDIRECT_STATUS=0");
		if (_request->_method == "POST")
		{
			std::string content_len = "CONTENT_LENGTH="+ std::to_string(_request->_body.size());
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


void	Answer::execute_cgi_request(){
	int pip_to_cgi[2];
	int pip_from_cgi[2];

	set_cgi_env(); // set env for cgi
	if (pipe(pip_to_cgi) != 0 || pipe(pip_from_cgi) != 0)
		perror("pipe");
	int pid = fork();
	if (pid == 0)
		send_to_cgi(pip_to_cgi, pip_from_cgi);
	else
		receive_from_cgi(pip_to_cgi, pip_from_cgi, pid);
}

void Answer::send_to_cgi(int pip_to_cgi[2], int pip_from_cgi[2]){
		char **argv = new char*[3];
		char *path = new char[5000];

		if (!getwd(path))
			perror("getwd");
		argv[0] = strdup(strcat(path, "/cgi/cgi_tester"));
		if (_request->_method == "POST")
			argv[1] = strdup(strcat(path, "/front-end/html/index2.html"));
		else
			argv[1] = strdup(strcat(path, "/front-end/html/index.html"));
		argv[2] = 0;
		if (dup2(pip_to_cgi[0], STDIN_FILENO) == -1)
			perror("Can't duplicate pip_to_cgi to STDIN_FILENO");
		if (dup2(pip_from_cgi[1], STDOUT_FILENO) == -1)
			perror("Can't duplicate pip_from_cgi to STDOUT_FILENO");
		close(pip_from_cgi[0]);
		close(pip_from_cgi[1]);
		close(pip_to_cgi[0]);
		close(pip_to_cgi[1]);
		if (execve(argv[0], argv, _cgi_env) == -1)
			perror("execve");
		delete []path;
		delete []argv; 
		delete []_cgi_env; // TODO: check if it needs to delete each char * individually
}

void Answer::receive_from_cgi(int pip_to_cgi[2], int pip_from_cgi[2], int pid){
	int status;
	close(pip_to_cgi[0]);
	close(pip_from_cgi[1]);
	if (_request->_method == "POST")
	{
		if (write(pip_to_cgi[1], _request->_body.c_str(), _request->_body.size()) == -1)
			perror("write");
	}
	close(pip_to_cgi[1]);
	pip_to_cgi[1] = -1;
	if (waitpid(pid, &status, 0) == -1)
		perror("waitpid");
	std::string cgi_answer = read_answer(pip_from_cgi[0]);
	split_cgi_answer(cgi_answer);
	std::cout << "cgi_status: " << _cgi_status << std::endl;
	std::cout << "cgi_header: " << _cgi_header << std::endl;
	std::cout << "cgi_body: " << _cgi_body << std::endl;
	close(pip_from_cgi[0]);
	delete _cgi_env; // TODO: check if it needs to delete each char * individually
}


void Answer::split_cgi_answer(std::string &answer){
	_cgi_status = answer.substr(0, answer.find("\r\n"));
    answer.erase(0, answer.find_first_of("\r\n") + 2);
    _cgi_header = answer.substr(0, answer.find_first_of("\r\n\r\n"));
    answer.erase(0, _cgi_header.size() + 4);
    _cgi_body = answer;
}