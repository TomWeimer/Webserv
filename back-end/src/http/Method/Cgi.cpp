#include "Cgi.hpp"

Cgi::Cgi(std::string cgi_file, std::string query_string)
{
	_cgi_file = cgi_file;
	_query = query_string;
	// set_path();
}

Cgi::~Cgi(){}

// void Cgi::set_path(){
// 	path = get
// }

void Cgi::execute_python_cgi(){
		
	int pip_to_cgi[2];
	int pip_from_cgi[2];

	if (pipe(pip_to_cgi) != 0 || pipe(pip_from_cgi) != 0)
	perror("pipe");

	int pid = fork();
	if (pid == 0)
	{
		char **argv = new char*[4];
		char *path = new char[5000];

		if (!getwd(path))
			perror("getwd");

		argv[0] = (char*)"/usr/bin/python3";
		argv[1] = (char*)(path + _cgi_file).c_str();
		argv[2] = (char*)_query.c_str();
		argv[3] = NULL;
		
		if (dup2(pip_to_cgi[0], STDIN_FILENO) == -1)
			perror("Can't duplicate pip_to_cgi to STDIN_FILENO");
		if (dup2(pip_from_cgi[1], STDOUT_FILENO) == -1)
			perror("Can't duplicate pip_from_cgi to STDOUT_FILENO");
		close(pip_from_cgi[0]);
		close(pip_from_cgi[1]);
		close(pip_to_cgi[0]);
		close(pip_to_cgi[1]);
		if (execve(argv[0], argv, NULL) == -1)
			perror("execve");
		delete []path;
		delete []argv; 
		// delete []_cgi_env; // TODO: check if it needs to delete each char * individually
	}
	else
	{
		int status;
		close(pip_to_cgi[0]);
		close(pip_from_cgi[1]);
		close(pip_to_cgi[1]);
		pip_to_cgi[1] = -1;
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
		std::string cgi_answer = read_answer(pip_from_cgi[0]);
		std::cout << "cgi_answer: " << cgi_answer << std::endl;
		// std::cout << "cgi_header: " << _cgi_header << std::endl;
		// std::cout << "cgi_body: " << _cgi_body << std::endl;
		close(pip_from_cgi[0]);
		// delete _cgi_env; // TODO: check if it needs to delete each char * individually
	}
}

std::string Cgi::read_answer(int fd)
{
    std::string ret;
    char buf[4096 + 1];
    int readed;

    while ((readed = read(fd, buf, 4096)) != 0)
    {
        if (readed == -1)
            return "";
        buf[readed] = '\0';
        ret.insert(ret.length(), buf, readed);
    }
    return ret;
}
