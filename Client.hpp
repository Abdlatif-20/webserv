/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 21:51:52 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/10 18:42:38 by aben-nei         ###   ########.fr       */
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
