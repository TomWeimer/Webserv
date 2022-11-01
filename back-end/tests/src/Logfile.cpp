#include "Logfile.hpp"
#include <cstdlib>
#include <cstring>
Logfile::Logfile(std::string name)
	: filename(name), path() {}

Logfile::Logfile(const Logfile& origin)
	: filename(origin.filename), path(origin.path) {}

Logfile &Logfile::operator=(const Logfile& origin)
{
	if (this == &origin)
		return (*this);
	if (fileIsOpen() == true)
		closeFile();
	this->filename = origin.filename;
	return (*this);
}

Logfile::~Logfile(void)	{}

bool Logfile::fileIsOpen(void)
{
	return (file.is_open());
}

void Logfile::openFile(void)
{
	std::string fullname;
	fullname = path;
	fullname += this->filename;
	file.open(fullname.c_str(), std::ofstream::out | std::ofstream::app);
}

void Logfile::closeFile(void)
{
	file.close();
}

void Logfile::writeToFile(const std::stringstream& content)
{
	file << content.str();
}

void Logfile::writeToFile(const KeyWord& content)
{
	std::string str;
	std::vector<Token>::const_iterator it;
	str = "KeyWord: ";
	str += content.tokenType;
	it = content.args.begin();
	while(it != content.args.end())
	{
		str += " token: ";
		str += it->tokenType;
		str += " value: ";
		str += it->valueToken;
		it++;
	}
	
	file.write(str.c_str(), str.size());
}

void Logfile::clear(void)
{
	std::string fullname;
	if (fileIsOpen())
		closeFile();
	fullname = path;
	fullname += filename;
	file.open(fullname.c_str(), std::ofstream::out | std::ofstream::trunc);
	closeFile();
}

void Logfile::operator<<(const std::stringstream& content)
{
	openFile();
	writeToFile(content);
	closeFile();
}


void Logfile::operator<<(const std::vector<KeyWord>& content)
{
	clear();
	openFile();
	for (std::vector<KeyWord>::const_iterator it = content.begin(); it != content.end(); it++)
	{
		writeToFile(*it);
	}
	closeFile();
}


void Logfile::set_path(const std::string& the_path)
{
	path = the_path;
}

std::string Logfile::get_filename() const
{
	return (filename);
}

std::string Logfile::get_fullname() const
{
	std::string fullname;

	fullname = path;
	fullname += filename;
	return (fullname);
}

bool compare_file(std::string filename_std, std::string filename_ft)
{
	std::ifstream 	infile_std;
	std::ifstream 	infile_ft;
	std::string		line[2];
	bool			testOk = true;

	infile_std.open(filename_std.c_str());
	infile_ft.open(filename_ft.c_str());
	while (infile_std.good() && infile_ft.good())
	{
		std::getline(infile_std, line[0]);
		std::getline(infile_ft, line[1]);
		if (line[0] != line[1])
		{
			testOk = false;
			break;
		}
		line[1].clear();
	}
	if (infile_ft.eof() != infile_std.eof())
		testOk = false;
	infile_std.close();
	infile_ft.close();
	return (testOk);
}