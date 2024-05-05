/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/05/05 12:06:55 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
	sockId = -1;
	requestDone = false;
	responseDone = false;
	last_update_time = std::time(NULL);
}

Client::Client(int sock)
{
	sockId = sock;
	requestDone = false;
	responseDone = false;
	last_update_time = std::time(NULL);
}

Client::Client(const Client &cpy)
{
	*this = cpy;
}

void	Client::reset(void)
{
	if (responseDone)
	{
		request.resetRequest();
		response.resetResponse();
	}
}

ssize_t Client::recvRequest(void)
{
	std::memset(buff, 0, sizeof(buff));
	len = recv(sockId, buff, sizeof(buff) - 1, 0);
	if (len < 0)
		throw (Fail("send fails"));
	request.parseRequest(std::string(buff, len), serverCTX);
	last_update_time = std::time(NULL);
	requestDone = request.isDone();
	responseDone = false;
	return (len);
}

void	Client::sendResponse(void)
{
	if (requestDone && !responseDone)
	{
		response.setRequest(&request);
		response.setServerCTX(request.getServerCTX());
		response.setLocationCTX(request.getLocationCtx());
		response.prepareResponse();
		if (!response.getHeadersSent() && response.headersToString() != "\r\n")
		{
			std::string headers = response.headersToString();
			response.getBody().insert(0, headers);
			response.setHeadersSent(true);
		}
		len = send(sockId, response.getBody().c_str(), response.getBody().size(), 0);// when fail? we should remove the client and continue :) // ok
		if (len < 0)
			throw (Fail("send fails"));
		responseDone = response.responseIsDone();
		last_update_time = std::time(NULL);
	}
}

void	Client::setServerCTX(const ServerContext& serverCTX)
{
	this->serverCTX = serverCTX;
}

bool	Client::isALive(void) const
{
	std::string	value;

	value = request.getHeaderByName("connection");
	if (value == "closed" || value == "close")
		return (false);
	return (true);
}

bool Client::timeout(void) const
{
	if (std::difftime(std::time(NULL), last_update_time) > 59)
		return (true);
	return (false);
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

time_t Client::getLastUpdateTime(void) const
{
	return (last_update_time);
}

void Client::requestTimeout(void)
{
	response.setRequest(&request);
	response.setServerCTX(request.getServerCTX());
	response.setLocationCTX(request.getLocationCtx());
	std::string	resp = response.requestTimeoutResponse();
	len = send(sockId, resp.c_str(), resp.size(), 0);
	if (len < 0)
		throw (Fail("send fails"));
	responseDone = response.responseIsDone();
	last_update_time = std::time(NULL);
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
		last_update_time = cpy.last_update_time;
	}
	return (*this);
}

Client::~Client(void)
{
	close(sockId);
}
