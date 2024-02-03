/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/03 00:40:55 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

class Utils
{
	private:
		Utils();
	public:
		static std::string strTrim(const std::string& str, char c);
		static std::vector<std::string> splitRequest(const std::string& str, const char *sep);
		static std::vector<std::string> split(const std::string& str, const char sep);
		static void		printVector(std::vector<std::string> vec);
		static void		printMap(const std::map<std::string, std::string>& map);
		static void		toLower(std::string& str);
};
int     runServer(char *av);