/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:55:56 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/20 18:58:34 by houmanso         ###   ########.fr       */
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
	std::cout << "Port "<< port << " => hosts [";
	for (StringVector::iterator i = serverNames.begin(); i != serverNames.end(); i++)
		std::cout << " " << *i;
	std::cout << " ]"<< std::endl;
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
}
