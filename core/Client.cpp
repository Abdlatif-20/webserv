/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/09 18:08:18 by houmanso         ###   ########.fr       */
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

int	Client::recvRequest(void)
{
	std::memset(buff, 0, sizeof(buff));
	len = recv(sockId, buff, 1023, 0);
	if (len)
		request.parseRequest(std::string(buff, len), serverCTX);
	else
		request.parseRequest("\r\n", serverCTX);
	requestDone = request.isDone();
	responseDone = false;
	std::cout << len << " => " << buff << std::endl;
	return (len);
}

void	Client::sendResponse(void)
{
	if (requestDone)
	{
		// send(sockId, "HTTP/1.1 200 OK\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd", 87, 0);
		response.setServerCtx(serverCTX);
		response.setRequest(request);
		std::string resp = response.generateResponse();
		send(sockId, resp.c_str(), resp.length(), 0);
		response.setStatusCode(0);
		requestDone = false;
		responseDone = true;
	}
}

void	Client::setServerCTX(const ServerContext& serverCTX)
{
	this->serverCTX = serverCTX;
}

bool	Client::hostIsDetected(void) const
{
	return (request.hostIsDetected());
}

bool Client::isRequestDone(void) const
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
		config = cpy.config;
		request = cpy.request;
		serverCTX = cpy.serverCTX;
	}
	return (*this);
}

Client::~Client(void)
{
	close(sockId);
}
