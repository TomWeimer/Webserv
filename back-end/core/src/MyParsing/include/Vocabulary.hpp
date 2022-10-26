#ifndef VOCABULARY_HPP
#define VOCABULARY_HPP
#include <map>
#include <utility>
#include <iostream>
#include <fstream>

class Vocabulary
{	
	public:
	typedef std::map<size_t, std::string>::iterator iterator;
	typedef std::map<size_t, std::string>::const_iterator const_iterator;
	private:
	std::map<std::string, std::string>  	_rules;
	std::map<size_t, std::string>			_names;

	public:
	Vocabulary(std::string filename);
	Vocabulary(const Vocabulary& origin);
	Vocabulary& operator=(const Vocabulary& origin);
	~Vocabulary();
	


	std::string			operator[](const int& index)const;
	std::string operator[](std::string& name)const;

	iterator		begin();
	const_iterator	begin()const;
	iterator		end();
	const_iterator	end()const;

	bool			empty()const;
	size_t 			size() const;



	private:
	void	createVocabulary(std::string filename);
	void	createNewEntry(std::ifstream& file, int index);
};

std::ostream& operator<<(std::ostream& out, const Vocabulary& vocabulary);
#endif