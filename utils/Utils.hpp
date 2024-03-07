/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/05 19:09:01 by mel-yous         ###   ########.fr       */
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

namespace Utils
{
	std::string	intToString(int number);
	std::string	strTrim(const std::string& str, char c);
	std::string getTokenNameFromDirective(t_directive d);
	std::string	getTokenNameFromDirective(t_directive d);
	std::string getDefaultErrorPage(const std::string& status);

	t_directive getDirectiveFromTokenName(const std::string& tokenName);

	std::vector<std::string> split(const std::string& str, const char sep);
	std::vector<std::string> splitRequest(const std::string& str, const char *sep);

	void		toLower(std::string& str);
	void		printFile(std::string filename);
	void		setupAddr(sockaddr_in *addr, int port);
	void		printVector(std::vector<std::string> vec);
	void		printMap(const std::map<std::string, std::string>& map);
	bool		stringStartsWith(const std::string& str, const std::string& prefix);

	std::string getCurrentTime();
	std::string readFile(const std::string& filePath);
};
