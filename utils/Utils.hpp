/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/23 00:07:59 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Defs.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
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
		int     runServer(char **av);
    	std::string strTrim(const std::string& str, char c);
    	std::string strTrim(const std::string& str, char c);
    	t_directive getDirectiveFromTokenName(const std::string& tokenName);
    	std::string getTokenNameFromDirective(t_directive d);
    	std::string getDefaultErrorPage(const std::string& status);
};
