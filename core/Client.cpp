/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/15 22:15:35 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
	sockId = -1;
	requestDone = false;
	responseDone = false;
	serverSelected = false;
}

Client::Client(int sock)
{
	sockId = sock;
	requestDone = false;
	responseDone = false;
	serverSelected = false;
}

Client::Client(const Client &cpy)
{
	*this = cpy;
}

void	Client::reset(void)
{
	request.resetRequest();
}

ssize_t Client::recvRequest(void)
{
	std::memset(buff, 0, sizeof(buff));
	len = recv(sockId, buff, 1023, 0);
	if (len > 0)
	{
		request.parseRequest(std::string(buff, len), &serverCTX);
		requestDone = request.isDone();
	}
	else
		requestDone = true;;
	responseDone = false;
	return (len);
}

void	Client::sendResponse(void)
{
	if (requestDone && !responseDone)
	{
		requestDone = false;
		responseDone = true;
		serverSelected = false;
		response.setRequest(&request);
		response.setContext(request.getContext());
		if (request.getMethod() == "GET")
			response.setMethod(0);
		else if (request.getMethod() == "POST")
			response.setMethod(1);
		else if (request.getMethod() == "DELETE")
			response.setMethod(2);
		response.prepareResponse();
		if (!response.getHeadersSent())
		{
			send(sockId, response.getHeaders().c_str(), response.getHeaders().size(), 0);
			response.setHeadersSent(true);
		}
		responseDone = response.responseIsDone();
		send(sockId, response.getBody().c_str(), response.getBody().size(), 0);
		reset(); // reset request and restponse;
	}
}

void	Client::setServerCTX(const ServerContext& serverCTX)
{
	this->serverCTX = serverCTX;
}

bool	Client::hostIsDetected(void) const
{
	return (serverSelected);
}

bool	Client::isALive(void) const
{
	std::string	value;

	value = request.getHeaderByName("connection");
	if (value == "closed")
		return (false);
	return (true);
}

bool Client::isRequestDone(void) const
{
	return (requestDone);
}

bool	Client::isResponseDone(void) const
{
	return (responseDone);
}


void Client::setServersEnd(size_t it)
{
	request.setServerCTXEnd(it);
}

void Client::setServersBegin(size_t it)
{
	request.setServerCTXBegin(it);
}

const Request &Client::getRequest(void) const
{
	return (request);
}

const Response &Client::getResponse(void) const
{
	return (response);
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
		request = cpy.request;
		response = cpy.response;
		serverCTX = cpy.serverCTX;
		requestDone = cpy.requestDone;
		responseDone = cpy.responseDone;
		serverSelected = cpy.serverSelected;
	}
	return (*this);
}

Client::~Client(void)
{
	close(sockId);
}
