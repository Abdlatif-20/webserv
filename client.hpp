/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 21:51:52 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/03 23:16:55 by aben-nei         ###   ########.fr       */
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
		std::vector<std::string> _clientHeaders;
		std::string _clientBody;
		std::string _clientMethod;
		std::string _clientPath;
		std::string _clientVersion;
		Request		_request;
		int _clientStatus;

	public:
		void parseRequest(std::string request);
		void parseHeaders(std::string headers);
		void fillHeaders(std::vector<std::string> headers);
};