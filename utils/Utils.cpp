/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:17:03 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/08 17:40:38 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::string Utils::strTrim(const std::string& str, char c)
{
	if (str.empty())
		throw "Error: empty string";
	size_t i = str.find_first_not_of(c);
	size_t j = str.find_last_not_of(c);
	if (i == std::string::npos)
		i = 0;
	if (j == std::string::npos)
		j = str.size();
	return str.substr(i, j - i + 1);
}
//POST / HTTP/1.1^M$
std::vector<std::string> Utils::splitRequest(const std::string& str, const char *sep)
{
	std::vector<std::string> result;
	size_t start = 0;
	size_t pos = str.find(sep);
	while (pos != std::string::npos)
	{
		std::string _sep = sep;
		result.push_back(str.substr(start, (pos - start)));
		start = pos + 2;
		pos = str.find(sep, start);
	}
	result.push_back(str.substr(start, (pos - start)));
	return result;
}

std::vector<std::string> Utils::split(const std::string& str, const char sep)
{
	std::vector<std::string> result;
	std::string token;
	std::string newStr = strTrim(str, '\r');
	std::istringstream tokenStream(newStr);
	while (std::getline(tokenStream, token, sep))
	{
		if(token.empty() || token == "\n" || token == "\r" || token == "\r\n" || token == "\n\r")
			continue; 
		result.push_back(token);
	}
	return result;
}

void    Utils::printMap(const std::map<std::string, std::string>& map)
{
	for (std::map<std::string, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
		std::cout << "header[" << it->first << "]: " << it->second << std::endl;
}

void    Utils::printVector(std::vector<std::string> vec)
{
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
		std::cout << "req[" << *it << "]" << std::endl;
}

void    Utils::toLower(std::string& str)
{
	std::string::iterator it;
	for (it = str.begin(); it != str.end(); it++)
		*it = tolower(*it);
}
