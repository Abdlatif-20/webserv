/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/10 19:09:23 by mel-yous         ###   ########.fr       */
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

Client::Client(const Client &cpy)
{
	*this = cpy;
}

ssize_t	Client::recvRequest(void)
{
	std::memset(buff, 0, sizeof(buff));
	len = recv(sockId, buff, 1023, 0);
	if (len < 0)
		return len;
	request.parseRequest(std::string(buff, len), &serverCTX);
	requestDone = request.isDone();
	responseDone = false;
	return (len);
}

void	Client::sendResponse(void)
{
	if (requestDone)
	{
		response.setRequest(&request);
		// requestDone = false;
		// responseDone = false;
		// send(sockId, "HTTP/1.1 200 OK\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd", 87, 0);
	}
}

void	Client::setServerCTX(const ServerContext& serverCTX)
{
	this->serverCTX = serverCTX;
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
		len = cpy.len;
		sockId = cpy.sockId;
		requestDone = cpy.requestDone;
		responseDone = cpy.responseDone;
		request = cpy.request;
		serverCTX = cpy.serverCTX;
	}
	return (*this);
}

Client::~Client(void)
{
	close(sockId);
}
