/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/24 20:52:31 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
	std::string envName(std::string str);
	std::string strTrim(const std::string& str, char c);
	std::vector<std::string> splitRequest(const std::string& str, const char *sep);
	std::vector<std::string> split(const std::string& str, const char sep);
	void		printVector(std::vector<std::string> vec);
	void		printMap(const std::map<std::string, std::string>& map);
	void		toLower(std::string& str);
	void		printFile(std::string filename);
	std::string intToString(int number);
	t_directive getDirectiveFromTokenName(const std::string& tokenName);
	std::string getTokenNameFromDirective(t_directive d);
	std::string getDefaultErrorPage(const std::string& status);
	int stringToInt(const std::string& str);
	void	decodeUri(std::string& uri);
	bool		stringStartsWith(const std::string& str, const std::string& prefix);
	bool stringEndsWith(const std::string& str, const std::string& suffix);
	std::string getCurrentTime();
	bool checkIfPathExists(const std::string& path);
	bool isDirectory(const std::string& path);
	bool isReadableFile(const std::string& path);
	std::string getFileExtension(const std::string& filePath);
	long long getFileSize(const std::string& filePath);
	std::string longlongToString(long long number);
	std::string replaceAll(std::string str, const std::string& s1, const std::string& s2);
	std::string get_last_modified_date(const std::string& path);
	class FileNotFoundException : public std::exception {};
	class FilePermissionDenied : public std::exception {};
	class PathIsDirectory : public std::exception {};
};
