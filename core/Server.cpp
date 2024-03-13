/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:55:56 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/12 17:19:39 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>

Server::Server(void)
{

}

Server::Server(const ServerContext &_serverCTX)
{
	struct addrinfo	hints;
	int ret;

	res = NULL;
	sockID = -1;
	serverCTX = _serverCTX;
	host = serverCTX.getHost();
	port = serverCTX.getPort();
	host_port = host + ":" + port;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((ret = getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) < 0)
		throw (Fail(gai_strerror(ret)));
	std::cout << "Setup Server " << host << ":" << port << " ...\n";
	serverNames = serverCTX.getServerName();
	serverNames.push_back(host_port);
	serverNames.push_back(host);
}

int	Server::getSocketId(void) const
{
	return (sockID);
}

void Server::setupServer(void)
{
	int a = 1;
	for (; res != NULL; res = res->ai_next)
	{
		sockID = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockID == -1)
			continue ;
		if (setsockopt(sockID, SOL_SOCKET, SO_REUSEADDR, &a, sizeof a) < 0
			|| setsockopt(sockID, SOL_SOCKET, SO_NOSIGPIPE, &a, sizeof a) < 0
			|| bind(sockID, res->ai_addr, res->ai_addrlen) < 0)
		{
			close(sockID);
			continue ;
		}
		break;
	}
	if (!res)
		throw(Fail("Failed to bind with any address"));
	if (listen(sockID, INT_MAX) == -1)
		throw(Fail(strerror(errno)));
	std::cout << "hosting " << host << ":" << port << " ...\n";
}

const std::string& Server::getHost(void) const
{
	return host;
}

const std::string& Server::getPort(void) const
{
	return	(port);
}

const std::string &Server::getHostPort(void) const
{
	return (host_port);
}

const StringVector &Server::getServerNames(void) const
{
	return (serverNames);
}

const ServerContext &Server::getServerCTX(void) const
{
	return (serverCTX);
}

Server::Server(const Server &cpy)
{
	*this = cpy;
}

Server &Server::operator=(const Server &cpy)
{
	if (this != &cpy)
	{
		res = cpy.res;
		host = cpy.host;
		port = cpy.port;
		sockID = cpy.sockID;
		host_port = cpy.host_port;
		serverCTX = cpy.serverCTX;
		serverNames = cpy.serverNames;
	}
	return (*this);
}

bool Server::operator<(const Server &cpy) const
{
	return (host_port < cpy.getHostPort());
}

bool Server::operator>(const Server &cpy) const
{
	return (host_port > cpy.getHostPort());
}

bool Server::operator<=(const Server &cpy) const
{
	return (host_port <= cpy.getHostPort());
}

bool Server::operator>=(const Server &cpy) const
{
	return (host_port > cpy.getHostPort());
}

bool Server::operator==(const Server &cpy) const
{
	return (host_port == cpy.getHostPort());
}

Server::~Server(void)
{
	if (sockID != -1)
		close(sockID);
}
