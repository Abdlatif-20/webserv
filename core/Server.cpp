/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:55:56 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/22 13:21:07 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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

#define JJJ "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd"


void Server::setupServer(void)
{
	sockID = socket(PF_INET, SOCK_STREAM, 0);
	if (sockID == -1)
		throw(Fail(strerror(errno)));
	std::cout << "Server: Socket : " << sockID << std::endl;
	if (bind(sockID, (sockaddr *)&addr, sizeof addr) == -1)
		throw(Fail(strerror(errno)));
	std::cout << "Server: Bind : " << sockID << std::endl;
	if (listen(sockID, INT_MAX) == -1)
		throw(Fail(strerror(errno)));
	std::cout << "Server: Listen : " << sockID << std::endl;
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
	{
		int a = 1;
		setsockopt(sockID, SOL_SOCKET, SO_REUSEADDR,&a, sizeof a);
		close(sockID);
		std::cout << "clossing socket :" << sockID << std::endl;
	}
	else
		std::cout << "tmp server" << std::endl;
}
