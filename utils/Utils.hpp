/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/29 17:41:51 by mel-yous         ###   ########.fr       */
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

namespace Utils
{
	std::string strTrim(const std::string& str, char c);
	std::vector<std::string> splitRequest(const std::string& str, const char *sep);
	std::vector<std::string> split(const std::string& str, const char sep);
	void		printVector(std::vector<std::string> vec);
	void		printMap(const std::map<std::string, std::string>& map);
	void		toLower(std::string& str);
	void		toUpper(std::string& str);
	void		printFile(std::string filename);
	std::string intToString(int number);
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
	std::string longlongToString(long long number);
	long long strToll(const std::string& str);
	std::string floatToString(float f);
	std::string replaceAll(std::string str, const std::string& s1, const std::string& s2);
	std::string get_last_modified_date(const std::string& path);
	std::string intToHex(int i);
	unsigned int hexToInt(const std::string& hex);
	std::string urlEncoding(const std::string& str);
	class FileNotFoundException : public std::exception {};
	class FilePermissionDenied : public std::exception {};
	class PathIsDirectory : public std::exception {};
};
