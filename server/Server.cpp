/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 23:29:26 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/05 17:43:59 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
	// nothing for now
}

Server::Server(const Server &cpy)
{
	*this = cpy;
}

Server &Server::operator=(const Server __unused &cpy)
{
	return (*this);
}

void Server::run(void)
{
	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID == -1)
		throw Utils::Fail("creating socket Failed");
	if (bind(socketID,(sockaddr*) &addr, sizeof addr) == -1)
		throw Utils::Fail("binding socket Failed");
	std::cout << "log: binded " << 6000 << std::endl;
	if (!listen(socketID, 1))
		std::cout << "listening ..." << std::endl;
	setupAddr();
	int clientsock = accept(socketID, NULL, 0);
	if (clientsock == -1)
		std::cout << "failed to accept" << std::endl;
	char msg[1024];
	bzero(msg, 1024);
	recv(clientsock, msg, 1024 , 0);
	std::cout << msg << std::endl;
}

// TODO: make addr any , and set port as config file said
void Server::setupAddr(void)
{
	std::memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000); // from config file
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

Server::~Server(void)
{
	close(socketID);
}
