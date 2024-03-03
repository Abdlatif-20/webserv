/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:55:56 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/02 19:32:54 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>

Server::Server(void)
{

}

Server::Server(const ServerContext &_serverCTX)
{
	sockID = -1;
	serverCTX = _serverCTX;
	host = serverCTX.getHost();
	port = serverCTX.getPort();
	std::cout << host << " " << port << '\n';
	serverNames = serverCTX.getServerName();
	Utils::setupAddr(&addr, std::atoi(port.c_str()));
}

int	Server::getSocketId(void) const
{
	return (sockID);
}

void Server::setupServer(void)
{
	sockID = socket(AF_INET, SOCK_STREAM, 0);
	if (sockID == -1)
		throw(Fail(strerror(errno)));
	int a = 1;
	setsockopt(sockID, SOL_SOCKET, SO_REUSEADDR, &a, sizeof a);
	setsockopt(sockID, SOL_SOCKET, SO_NOSIGPIPE, &a, sizeof a);
	if (bind(sockID, (sockaddr *)&addr, sizeof addr) == -1)
		throw(Fail(strerror(errno)));
	if (listen(sockID, INT_MAX) == -1)
		throw(Fail(strerror(errno)));
}

const std::string& Server::getHost(void) const
{
	return host;
}

const std::string& Server::getPort(void) const
{
	return	(port);
}

const ServerContext& Server::getServerCTX(void) const
{
	return serverCTX;
}

Server::Server(const Server &cpy)
{
	*this = cpy;
}

Server &Server::operator=(const Server &cpy)
{
	if (this != &cpy)
	{
		sockID = cpy.sockID;
		serverCTX = cpy.serverCTX;
		addr = cpy.addr;
		host = cpy.host;
		port = cpy.port;
		serverNames = cpy.serverNames;
	}
	return (*this);
}

Server::~Server(void)
{
	if (sockID != -1)
		close(sockID);
}
