/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:51:06 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/01 09:46:15 by houmanso         ###   ########.fr       */
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
	requestDone = true;
	if (len <= -1)
		return (-1);
	buff[len] = '\0';
	ss << len;
	return (len);
}

void	Client::sendResponse(void)
{
	if (requestDone)
		send(sockId, "HTTP/1.1 200 OK\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd", 87, 0);
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
