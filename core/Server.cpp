/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:55:56 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/01 09:47:21 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>

Server::Server(void)
{
}

Server::Server(const ServerContext &_serverCTX)
{
	HostPort hp;

	serverCTX = _serverCTX;
	hp = Utils::getPortAndHost(serverCTX.getListen());
	if (!hp.first.empty())
		serverNames.push_back(hp.first);
	StringVector tmp = serverCTX.getServerName();
	serverNames.insert(serverNames.end(), tmp.begin(), tmp.end());
	host = hp.first;
	port = hp.second;
	sockID = -1;
	Utils::setupAddr(&addr, std::strtol(port.c_str(), 0, 10));
}

int	Server::getSocketId(void) const
{
	return (sockID);
}

void Server::setupServer(void)
{
	sockID = socket(PF_INET, SOCK_STREAM, 0);
	if (sockID == -1)
		throw(Fail(strerror(errno)));
	int a = 1;
	setsockopt(sockID, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
	if (bind(sockID, (sockaddr *)&addr, sizeof addr) == -1)
		throw(Fail(strerror(errno)));
	if (listen(sockID, INT_MAX) == -1)
		throw(Fail(strerror(errno)));
}

std::string	Server::getPort(void) const
{
	return	(port);
}

Server::Server(const Server &cpy)
{
	*this = cpy;
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
	if (sockID != -1)
		close(sockID);
}
