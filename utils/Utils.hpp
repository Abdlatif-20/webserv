/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/18 14:21:18 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Defs.hpp"

namespace Utils
{
	std::string	strTrim(const std::string& str, char c);
	std::string	getTokenNameFromDirective(t_directive d);

	t_directive	getDirectiveFromTokenName(const std::string& tokenName);

	// reset the sockaddrs setup it with the config file
	void	setupAddr(sockaddr_in *addr, int port);

	HostPort	getPortAndHost(const std::string& str);
};