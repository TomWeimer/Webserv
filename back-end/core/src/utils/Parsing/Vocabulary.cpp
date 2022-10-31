#include "utils/Parsing/Vocabulary.hpp"

Vocabulary::Vocabulary(std::string filename)
{
	createVocabulary(filename);
}

Vocabulary::Vocabulary(const Vocabulary& origin)
 : _rules(origin._rules), _names(origin._names) {}

Vocabulary& Vocabulary::operator=(const Vocabulary& origin)
{
	if (this == &origin)
		return (*this);
	_rules = origin._rules;
	_names = origin._names;
	return(*this);
}

Vocabulary::~Vocabulary() {}

std::string Vocabulary::content(size_t i)
{
	std::string ruleName;
	std::string ruleContent;

	if (i >= _names.size())
		return ("");
	ruleName = _names[i];
	ruleContent = _rules[ruleName];
	return (ruleContent);
}

std::string Vocabulary::name(size_t i)
{
	std::string ruleName;

	if (i >= _names.size())
		return ("");
	ruleName = _names[i];
	return (ruleName);
}

void Vocabulary::createVocabulary(std::string filename)
{
	std::ifstream file;
	std::string test;
	int i;

	i = 0;
	file.open(filename.c_str());
	if (file.fail() == true)
		std::cout << "failbit" << std::endl;
	if (file.good() == false)
		throw (std::runtime_error("The .ebnf file could'nt be opened\n"));
	while (file.eof() == false)
	{
		createNewEntry(file, i);
		i++;
	}
	file.close();

}

void Vocabulary::createNewEntry(std::ifstream& file, int index)
{
	std::string name;
	std::string rule;
	std::string tmp;

	file >> name;
	if (index == 0 && name.empty() == true)
		throw (std::runtime_error("The .ebnf file was empty!\n"));
	// skip '='
	file >> tmp;
	tmp.clear();
	std::getline(file, rule);
	if (name.empty() == false)
		this->_names.insert(std::make_pair(index, name));
	if (rule.empty() == false)
		this->_rules.insert(std::make_pair(name, rule));
}

Vocabulary::iterator Vocabulary::begin()
{
	return (_names.begin());
}

Vocabulary::const_iterator Vocabulary::begin()const
{
	return (_names.begin());
}


Vocabulary::iterator Vocabulary::end()
{
	return (_names.end());
}

size_t Vocabulary::size() const
{
	return (_names.size());
}

Vocabulary::const_iterator Vocabulary::end()const
{
	return (_names.end());
}

std::string Vocabulary::operator[](std::string& name)const
{
	std::map<std::string, std::string>::const_iterator result;
	std::string str;
	size_t end_char;

	end_char = name.find_last_of('\0');
	if (end_char != std::string::npos)
		name.erase(end_char);
	result = _rules.find(name);
	if (result == _rules.end())
	{
		std::cerr <<"name: " << name << "found nothing" << std::endl;
		return ("");
	}
	// std::cerr << "return" << result->second << std::endl;
	// exit(1);
	return (result->second);
}

std::string Vocabulary::operator[](const int& index)const
{
	const_iterator result;

	result = _names.find(index);
	if (result == _names.end())
		return("");
	return (result->second);
}

bool Vocabulary::empty()const
{
	return (_names.empty());
}

std::ostream& operator<<(std::ostream& out, const Vocabulary& vocabulary)
{
	std::string nameToken;
	for(Vocabulary::const_iterator first = vocabulary.begin(); first != vocabulary.end(); first++)
	{
		nameToken = first->second;
		out << nameToken << " = " << vocabulary[nameToken] << std::endl;
	}
	return (out);
}