/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 21:51:52 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/20 14:11:34 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <poll.h>
#include <algorithm>
#include "Request.hpp"

class Client
{
	private:
		Request request;
		int _clientStatus;

	public:
		Client();
		~Client();
		void parseRequest(std::string receivedRequest, char *configPath);
		// void parseRequest(std::string request);
		// void parseHeaders(std::string headers);
		// void fillHeaders(std::vector<std::string> headers);
		
};
