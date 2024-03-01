/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/29 10:34:39 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

namespace Utils
{
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
	bool stringStartsWith(const std::string& str, const std::string& prefix);
};
