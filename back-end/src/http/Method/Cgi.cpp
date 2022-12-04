#include "Cgi.hpp"

Cgi::Cgi(std::string cgi_file, std::string query_string)
{
	_cgi_file = cgi_file;
	_query = query_string;
	set_path();
}

Cgi::~Cgi(){}

void Cgi::execute_python_cgi(){
		
	int pip_to_cgi[2];
	int pip_from_cgi[2];

	if (pipe(pip_to_cgi) != 0 || pipe(pip_from_cgi) != 0)
	perror("pipe");

	int pid = fork();
	if (pid == 0)
		send_to_cgi(pip_to_cgi, pip_from_cgi);
	else
		recv_from_cgi(pip_to_cgi, pip_from_cgi, pid);
}

void Cgi::send_to_cgi(int pip_to_cgi[2], int pip_from_cgi[2]){

		if (dup2(pip_to_cgi[0], STDIN_FILENO) == -1)
			perror("Can't duplicate pip_to_cgi to STDIN_FILENO");
		if (dup2(pip_from_cgi[1], STDOUT_FILENO) == -1)
			perror("Can't duplicate pip_from_cgi to STDOUT_FILENO");
		close(pip_from_cgi[0]);
		close(pip_from_cgi[1]);
		close(pip_to_cgi[0]);
		close(pip_to_cgi[1]);
		char **argv = make_argv(PYTHON);
		if (execve(argv[0], argv, NULL) == -1)
			perror("execve");
		if (argv)
			delete argv;
}

void Cgi::recv_from_cgi(int pip_to_cgi[2], int pip_from_cgi[2], int pid){
	int status;
	close(pip_to_cgi[0]);
	close(pip_from_cgi[1]);
	close(pip_to_cgi[1]);
	pip_to_cgi[1] = -1;
	if (waitpid(pid, &status, 0) == -1)
		perror("waitpid");
	std::string cgi_answer = read_answer(pip_from_cgi[0]);
	std::cout << "cgi_answer: " << cgi_answer << std::endl;
	close(pip_from_cgi[0]);
}

char **Cgi::make_argv(int cgi_type){
	char **argv = new char*[4];

	if (cgi_type == PYTHON){
		argv[0] = (char*)"/usr/bin/python3";
		argv[1] = (char*)(_path + _cgi_file).c_str();
		argv[2] = (char*)_query.c_str();
		argv[3] = NULL;
		return argv;
	}
	return NULL;
}

void	Cgi::set_path(){
	char *path = new char[5000];

	if (!getwd(path))
		perror("getwd");

	_path = path;
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
