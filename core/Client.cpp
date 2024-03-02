/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:51:06 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/01 15:07:16 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
	sockId = -1;
	requestDone = false;
	responseDone = false;
}

Client::Client(int sock)
{
	sockId = sock;
	requestDone = false;
	responseDone = false;
}

// Client::Client(const Server &serverCTX)
// {
// 	this->serverCTX = serverCTX;
// }

Client::Client(const Client &cpy)
{
	*this = cpy;
}

int	Client::recvRequest(void)
{
	bzero(buff, sizeof(buff));
	len = recv(sockId, buff, 1023, 0);
	// if (len < 0)
	// {
	// 	buff[0] = '\0';
	// 	return (-1);
	// }
	// buff[len] = '\0';
	std::cout << "buff:\n" << buff << std::endl;
	request.parseRequest(buff, config);
	// requestDone = true;
	// ss << len;
	return (len);
}

void	Client::sendResponse(void)
{
	if (requestDone)
		send(sockId, "HTTP/1.1 200 OK\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd", 87, 0);
}

void	Client::setConfig(const Config& conf)
{
	config = conf;
}

void	Client::setSockId(int sock)
{
	sockId = sock;
}

bool	Client::requestIsDone(void) const
{
	return (requestDone);
}

Client	&Client::operator=(const Client &cpy)
{
	if (this != &cpy)
	{
		sockId = cpy.sockId;
		requestDone = false;
		responseDone = false;
		// serverCTX = cpy.serverCTX;
	}
	return (*this);
}

Client::~Client(void)
{
	close(sockId);
}
