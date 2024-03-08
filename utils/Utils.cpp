/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:17:03 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/07 20:26:36 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::string Utils::strTrim(const std::string& str, char c)
{
	if (str.empty())
		return str;
	size_t i = str.find_first_not_of(c);
	size_t j = str.find_last_not_of(c);
	if (i == std::string::npos)
		i = 0;
	if (j == std::string::npos)
		j = str.size();
	return str.substr(i, j - i + 1);
}

t_directive Utils::getDirectiveFromTokenName(const std::string& tokenName)
{
	std::string tokens[11] = 
	{
		"root", "index", "auto_index", "error_page", "client_max_body_size",
		"allowed_methods", "listen", "server_name", "return", "location", "upload_store"
	};
	for (int i = 0; i < 11; i++)
		if (tokenName == tokens[i])
			return static_cast<t_directive>(i);
	return UNKNOWN;
}

std::string Utils::getDefaultErrorPage(const std::string& status)
{
	std::string errors[12] =
	{
		"400", "403", "404", "405", "408", "411",
		"414", "429", "500", "501", "502", "505"
	};
	for (size_t i = 0; i < 12; i++)
		if (status == errors[i])
			return "assets/www/error/" + status + ".html";
	return "";
}

std::vector<std::string> Utils::splitRequest(const std::string& str, const char *sep)
{
	std::vector<std::string> result;
	size_t start = 0;
	size_t pos = str.find(sep);
	while (pos != std::string::npos)
	{
		std::string _sep = sep;
		result.push_back(str.substr(start, (pos - start)));
		start = pos + _sep.size();
		pos = str.find(sep, start);
	}
	result.push_back(str.substr(start, (pos - start)));
	return result;
}

std::vector<std::string> Utils::split(const std::string& str, const char sep)
{
	std::vector<std::string> result;
	std::string token;
	std::istringstream tokenStream(str);
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
		std::cout << "header[" << it->first << "]: " << "value[" << it->second << "]" << std::endl;
}

void    Utils::printVector(std::vector<std::string> vec)
{
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
		std::cout << "req[" << *it << "]";
}

void    Utils::toLower(std::string& str)
{
	std::string::iterator it;
	for (it = str.begin(); it != str.end(); it++)
		*it = tolower(*it);
}

void		Utils::printFile(std::string filename)
{
	std::ifstream file(filename);
	std::string str;
	if (file.is_open())
	{
		while (std::getline(file, str))
			std::cout << str << std::endl;
		file.close();
	}
	else
		std::cout << "Unable to open file" << std::endl;
}

std::string Utils::intToString(int number)
{
	std::ostringstream oss;
	oss << number;
	return oss.str();
}

int Utils::stringToInt(const std::string& str)
{
	std::istringstream iss(str);
	int result;
	iss >> result;
	return result;
}

void	Utils::setupAddr(sockaddr_in *addr, int port)
{
	std::memset(addr, 0, sizeof(sockaddr_in));
	addr->sin_port = htons(port);
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
}

std::string Utils::getTokenNameFromDirective(t_directive d)
{
	switch (d)
	{
		case ROOT:
			return "root";
		case INDEX:
			return "index";
		case AUTO_INDEX:
			return "auto_index";
		case ERROR_PAGE:
			return "error_page";
		case CLIENT_MAX_BODY_SIZE:
			return "client_max_body_size";
		case ALLOWED_METHODS:
			return "allowed_methods";
		case LISTEN:
			return "listen";
		case SERVER_NAME:
			return "server_name";
		case RETURN:
			return "return";
		case LOCATION:
			return "location";
		case UPLOAD_STORE:
			return "upload_store";
		default:
			break;
	}
	return "unknown";
}

void	Utils::decodeUri(std::string& uri)
{
	std::string decoded;
	for (size_t i = 0; i < uri.size(); i++)
	{
		if (uri[i] == '%' && i + 2 < uri.size())
		{
			std::string hex = uri.substr(i + 1, 2);
			char c = (char)std::strtol(hex.c_str(), 0, 16);
			decoded += c;
			i += 2;
		}
		else
			decoded += uri[i];
	}
	uri = decoded;
}

bool Utils::stringStartsWith(const std::string& str, const std::string& prefix)
{
	return (!std::strncmp(str.c_str(), prefix.c_str(), prefix.length()));
}

std::string Utils::getCurrentTime()
{
    std::string strTime;
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    int year, month, day, hour, min, sec;
    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
    hour = now->tm_hour;
    min = now->tm_min;
    sec = now->tm_sec;
    strTime = Utils::intToString(year) + "-" + Utils::intToString(month) + "-" + Utils::intToString(day);
    strTime += " " + Utils::intToString(hour) + ":" + Utils::intToString(min) + ":" + Utils::intToString(sec);
    return strTime;
}

std::string Utils::readFile(const std::string& filePath)
{
    std::string line;
    struct stat statBuffer;
    if (stat(filePath.c_str(), &statBuffer) == -1)
        throw FileNotFoundException();
    if (S_ISDIR(statBuffer.st_mode) || !(S_IRUSR & statBuffer.st_mode))
        throw FilePermissionDenied();
    std::ifstream ifs(filePath);
    std::string text;
    if (!ifs.good())
        throw FileNotFoundException();
    while (std::getline(ifs, line))
        text += line;
    return text;
}

std::string Utils::getFileExtension(const std::string& filePath)
{
    size_t i = filePath.find_last_of('.');
    if (i != std::string::npos)
        return filePath.substr(i, filePath.length());
    return "";
}