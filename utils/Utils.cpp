/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:17:03 by mel-yous          #+#    #+#             */
/*   Updated: 2024/05/01 13:11:02 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::string OS = "macOS";

std::string Utils::envName(std::string str)
{
	std::string::iterator	it;

	it = str.begin();
	while (it != str.end())
	{
		*it = std::toupper(*it);
		if (*it == '-')
			*it = '_';
		it++;
	}
	return ("HTTP_" + str);
}

std::string Utils::strTrim(const std::string &str, char c)
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
	std::string tokens[13] = 
	{
		"root", "index", "auto_index", "error_page", "client_max_body_size",
		"allowed_methods", "listen", "server_name", "return", "location",
		"upload_store", "cgi", "cgi_max_timeout"
	};
	for (int i = 0; i < 13; i++)
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
	std::cout << "Printing map" << std::endl;
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

void    Utils::toUpper(std::string& str)
{
	std::string::iterator it;
	for (it = str.begin(); it != str.end(); it++)
		*it = toupper(*it);
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

int Utils::stringToInt(const std::string& str)
{
	std::istringstream iss(str);
	int result;
	iss >> result;
	return result;
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
		case CGI_ASSIGN:
			return "cgi";
		case CGI_MAX_TIMEOUT:
			return "cgi_max_timeout";
		default:
			break;
	}
	return "unknown";
}

std::string	Utils::urlDecoding(const std::string& uri)
{
	std::string decodedStr;
	std::string hex;
	for (size_t i = 0; i < uri.length(); i++)
	{
		if (uri[i] == '%')
		{
			hex = uri.substr(i + 1, 2);
			decodedStr += (char)hexToInt(hex);
			i += 2;
		}
		else
			decodedStr += uri[i];
	}
	return decodedStr;
}

bool Utils::stringStartsWith(const std::string& str, const std::string& prefix)
{
	return (!std::strncmp(str.c_str(), prefix.c_str(), prefix.length()));
}

bool Utils::stringEndsWith(const std::string& str, const std::string& suffix)
{
	if (suffix.size() > str.size())
		return false;
	return !str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

std::string Utils::getCurrentTime()
{
	char buffer[128];
	time_t rawTime;
	struct tm timeinfo;

	time(&rawTime);
	gmtime_r(&rawTime, &timeinfo);
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &timeinfo);
	return std::string(buffer);
}

bool Utils::checkIfPathExists(const std::string& path)
{
	struct stat statBuff;
	if (stat(path.c_str(), &statBuff) < 0)
	{
		if (errno == EACCES)
			throw FilePermissionDenied();
		return false;
	}
	return true;
}

bool Utils::isDirectory(const std::string& path)
{
	struct stat statBuff;
	if (!stat(path.c_str(), &statBuff) && S_ISDIR(statBuff.st_mode))
		return true;
	return false;
}

bool Utils::isReadableFile(const std::string& path)
{
	struct stat statBuff;
	if (!stat(path.c_str(), &statBuff) && (statBuff.st_mode & S_IRUSR))
		return true;
	return false;
}

std::string Utils::getFileExtension(const std::string& filePath)
{
    size_t i = filePath.find_last_of('.');
    if (i != std::string::npos)
        return filePath.substr(i, filePath.length());
    return "";
}

long long Utils::getFileSize(const std::string& filePath)
{
	struct stat statBuff;
	if (stat(filePath.c_str(), &statBuff) < 0 || isDirectory(filePath))
		return -1;
	return statBuff.st_size;
}

static std::string formatFloat(const std::string& str)
{
	size_t i;
	i = str.find('.');
	if (i != std::string::npos)
		return str.substr(0, i + 3);
	return str;
}

std::string Utils::bytesToHuman(long long bytes)
{
	std::string str;
	if (bytes < 1000)
		str = formatFloat(numberToString(bytes)) + " B";
	else if (bytes < 1000000)
		str = formatFloat(numberToString((float)bytes * 0.001)) + " KB";
	else if (bytes < 1000000000)
		str = formatFloat(numberToString((float)bytes * 0.000001)) + " MB";
	else
		str = formatFloat(numberToString((float)bytes * 0.000000001)) + " GB";
	return str;
}

long long Utils::strToll(const std::string& str)
{
	std::stringstream ss;
	long long num;
	ss << str;
	ss >> num;
	return num;
}

std::string Utils::replaceAll(std::string str, const std::string& s1, const std::string& s2)
{
	size_t pos = 0;
	while ((pos = str.find(s1, pos)) != std::string::npos) {
		str.erase(pos, s1.length());
		str.insert(pos, s2);
		pos += s2.length();
  	}
	return str;
}

std::string Utils::get_last_modified_date(const std::string& path)
{
	struct stat result;
	char buffer[128];
	if (stat(path.c_str(), &result) != 0)
		return "--";
	time_t modified_time = result.st_mtime;
	struct tm timeinfo;
	gmtime_r(&modified_time, &timeinfo);
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &timeinfo);
	return std::string(buffer);
}

std::string Utils::intToHex(int i)
{
	std::stringstream ss;
	ss << std::hex << i;
	return ss.str();
}

unsigned int Utils::hexToInt(const std::string& hex)
{
	if (hex.empty())
		return (0);
	unsigned int decimalNumber;

	std::stringstream ss;
	ss << std::hex << hex;
	ss >> decimalNumber;
	return decimalNumber;
}

std::string Utils::urlEncoding(const std::string& str)
{
	std::string encodedStr;
	std::string hex;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (isascii(str[i]) && !std::isalnum(str[i]) && str[i] != '-'
			&& str[i] != '_' && str[i] != '.' && str[i] != '~')
		{
			hex = intToHex((int)str[i]);
			hex.length() == 1 ? encodedStr += "%0" + hex : encodedStr += "%" + hex;
		}
		else
			encodedStr += str[i];
	}
	return encodedStr;
}

void Utils::deleteFolderContent(const std::string& path)
{
	DIR* currentDir = opendir(path.c_str());
	dirent* dp;
	std::string targetFile;
	if (!isReadableFile(path))
		throw FilePermissionDenied();
	if (!currentDir)
		return;
	while ((dp = readdir(currentDir)))
	{
		targetFile = path + "/" + dp->d_name;
		if (stringEndsWith(targetFile, ".") || stringEndsWith(targetFile, ".."))
			continue;
		if (!isDirectory(targetFile))
			std::remove(targetFile.c_str());
		else
			deleteFolderContent(targetFile);
	}
	closedir(currentDir);
}

std::string Utils::getOperatingSystem()
{
	#ifdef __APPLE__
		return "macOS";
	#elif defined(__linux__)
		return "Linux";
	#elif defined(_WIN32)
		return "Windows";
	#endif
	return "Unix";
}