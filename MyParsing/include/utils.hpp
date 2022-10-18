#ifndef UTILS_HPP
#define UTILS_HPP
#include <cstring>

template<class Iterator>
void skip_occurrence(Iterator &first, Iterator &last, char c)
{
	for (; first != last && *first == c; first++)
	{}
}

template<class Iterator>
void skip_occurrence(Iterator &first, Iterator &last, bool(*f)(int c))
{
	for (; first != last && f(*first) == true; first++)
	{}
}

template<class Iterator>
void skip_occurrence(Iterator &first, Iterator &last, int(*f)(int c))
{
	for (; first != last && f(*first); first++)
	{}
}

template<class Iterator>
void skip_until(Iterator &first, Iterator &last, char c)
{
	for (; first != last && *first != c; first++)
	{}
}

template<class Iterator>
void skip_until_space(Iterator &first, Iterator &last)
{
	for (; first != last && std::isspace(*first) == false; first++)
	{}
}


template<class Iterator>
void skip_and_erase_space(std::string &content, Iterator &first, Iterator &last)
{
	int nb = 0;

	for (; first != last && std::isspace(*first) == false; first++)
	{
		nb++;
	}
	content.erase(0, nb);
}

template<class Iterator>
void skip_and_erase_until_space(std::string &content, Iterator &first, Iterator &last)
{
	int nb = 0;
	for (; first != last && std::isspace(*first) == false; first++)
	{
		nb++;
	}
	content.erase(0, nb);
}
template<class Iterator>
void skip_and_erase_while_space(std::string &content, Iterator &first, Iterator &last)
{
	int nb = 0;
	for (; first != last && std::isspace(*first) == true; first++)
	{
		nb++;
	}
	content.erase(0, nb);
	
}
template<class Iterator>
bool only_whitespace(Iterator first, Iterator last)
{
	for (; first != last; first++)
	{
		if (std::isspace(*first) == false)
			return (false);
	}
	return (true);
}
#endif