/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:55:56 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/17 20:09:35 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
}

Server::Server(const ServerContext &_serverCTX)
{
	serverCTX = _serverCTX;
	
	// std::memset(&addr, 0, sizeof addr);
	// addr.sin_family = AF_INET;
	// addr.sin_port =
	// sockID = socket(AF_INET, SOCK_STREAM, 0);
	// if ()
}

Server::Server(const Server &cpy)
{
	*this = cpy;
}

std::string Server::getPORT_tmp(void) const
{
	return (serverCTX.getListen());
}

Server &Server::operator=(const Server &cpy)
{
	if (this != &cpy)
	{
		port = cpy.port;
		addr = cpy.addr;
		sockID = cpy.sockID;
		serverCTX = cpy.serverCTX;
	}
	return (*this);
}

Server::~Server(void)
{
}
