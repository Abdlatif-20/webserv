/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:51:06 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/01 17:32:10 by houmanso         ###   ########.fr       */
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
	len = recv(sockId, buff, 1023, 0);
	if (len < 0)
	{
		buff[0] = '\0';
		return (-1);
	}
	buff[len] = '\0';
	std::cout << "buff=>" <<  buff << std::endl;
	request.parseRequest(buff, config);
	requestDone = true;
	// ss << len;
	return (len);
}

void	Client::sendResponse(void)
{
	std::cout << "sending" << std::endl;
	send(sockId, "HTTP/1.1 200 OK\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd", 87, 0);
	responseDone = true;
}

void	Client::setConfig(const Config& conf)
{
	config = conf;
}

bool	Client::isRequestDone(void) const
{
	return (requestDone);
}

bool	Client::isResponseDone(void) const
{
	return (responseDone);
}

void	Client::setSockId(int sock)
{
	sockId = sock;
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
