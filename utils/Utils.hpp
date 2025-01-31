/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/05/01 13:11:08 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Defs.hpp"
#include "Defs.hpp"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include <ctime>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

namespace Utils
{
	template <typename T>
	std::string numberToString(T num)
	{
		std::stringstream ss;
		ss << num;
		return ss.str();
	}
	std::string envName(std::string str);
	std::string strTrim(const std::string& str, char c);
	std::vector<std::string> splitRequest(const std::string& str, const char *sep);
	std::vector<std::string> split(const std::string& str, const char sep);
	void		printVector(std::vector<std::string> vec);
	void		printMap(const std::map<std::string, std::string>& map);
	void		toLower(std::string& str);
	void		toUpper(std::string& str);
	void		printFile(std::string filename);
	t_directive getDirectiveFromTokenName(const std::string& tokenName);
	std::string getTokenNameFromDirective(t_directive d);
	std::string getDefaultErrorPage(const std::string& status);
	int stringToInt(const std::string& str);
	std::string	urlDecoding(const std::string& uri);
	bool		stringStartsWith(const std::string& str, const std::string& prefix);
	bool stringEndsWith(const std::string& str, const std::string& suffix);
	std::string getCurrentTime();
	bool checkIfPathExists(const std::string& path);
	bool isDirectory(const std::string& path);
	bool isReadableFile(const std::string& path);
	std::string getFileExtension(const std::string& filePath);
	long long getFileSize(const std::string& filePath);
	std::string bytesToHuman(long long bytes);
	long long strToll(const std::string& str);
	std::string replaceAll(std::string str, const std::string& s1, const std::string& s2);
	std::string get_last_modified_date(const std::string& path);
	std::string intToHex(int i);
	unsigned int hexToInt(const std::string& hex);
	void deleteFolderContent(const std::string& path);
	std::string urlEncoding(const std::string& str);
	std::string getOperatingSystem();
	class FileNotFoundException : public std::exception {};
	class FilePermissionDenied : public std::exception {};
	class PathIsDirectory : public std::exception {};
};
