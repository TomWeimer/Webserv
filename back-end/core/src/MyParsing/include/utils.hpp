#ifndef UTILS_HPP
#define UTILS_HPP
#include <cstring>
#include "Operator.hpp"
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
void skip_while_space(Iterator &first, Iterator &last)
{
	for (; first != last && std::isspace(*first) == true; first++)
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

template<class Iterator>
size_t number_of_whitespace(Iterator first, Iterator last)
{
	size_t nb;

	nb = 0;
	for (; first != last; first++)
	{
		if (std::isspace(*first) == true)
			nb++;
	}
	return (nb);
}

template<class Iterator>
size_t size_without_whitespace(Iterator first, Iterator last)
{
	size_t nb;

	nb = 0;
	for (; first != last; first++)
	{
		if (std::isspace(*first) == false)
			nb++;
	}
	return (nb);
}

template<class Iterator>
std::string new_token(Iterator first, Iterator last)
{
	std::string::iterator start;
	std::string::iterator end;

	start = first;
	for (;start != last && std::isspace(*start) == true; start++)
	{}
	end = start;
	for(;end != last && std::isspace(*end) == false; end++)
	{}
	return(std::string(start, end));
}

template<class Iterator>
std::string new_keyWord(Iterator first, Iterator last)
{
	std::string::iterator start;
	std::string::iterator end;

	start = first;
	for (;start != last && std::isspace(*start) == true; start++)
	{}
	end = last;
	for(;end != start && std::isspace(*end) == true; end--)
	{}
	return(std::string(start, end));
}

template<class Iterator>
std::string find_next_token_between_operator(Iterator& first, Iterator last)
{
	std::string::iterator start_token;
	Operator type;
	
	start_token = first;
	type = is_operator(*first);
	while (first != last && (type <= 0 || type == SECONDQUOTE))
	{
		first++;
		type = is_operator(*first);
	}
	return (new_token(start_token, first));
}





template<class Iterator>
Operator find_next_operator(Iterator &first, Iterator last)
{
	while (first != last && is_operator(*first) <= 0)
	{
		//std::cerr << "->"  << *first << "<-" << std::endl;
		first++;
	}
	if (first != last)
		return (is_operator(*first));
	return (NONE);
}

#endif