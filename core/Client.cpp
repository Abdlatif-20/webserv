/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/24 18:25:18 by houmanso         ###   ########.fr       */
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
	len = recv(sockId, buff, 1023, 0);
	if (len > 0)
	{
		request.parseRequest(std::string(buff, len), serverCTX);
		requestDone = request.isDone();
		// last_update_time = std::time(NULL); to think about later
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
		response.setRequest(&request);
		response.setServerCTX(request.getServerCTX());
		response.setLocationCTX(request.getLocationCtx());
		response.prepareResponse();
		if (!response.getHeadersSent())
		{
			len = send(sockId, response.getHeaders().c_str(), response.getHeaders().size(), 0);
			response.setHeadersSent(true);
		}
		len = send(sockId, response.getBody().c_str(), response.getBody().size(), 0);// when fail?
		responseDone = response.responseIsDone();
		last_update_time = std::time(NULL);
	}
	reset();
}

void	Client::setServerCTX(const ServerContext& serverCTX)
{
	this->serverCTX = serverCTX;
}

bool	Client::isALive(void) const
{
	std::string	value;

	value = request.getHeaderByName("connection");
	if (value == "closed")
		return (false);
	return (true);
}

bool Client::timeout(void) const
{
	if (std::difftime(std::time(NULL), last_update_time) > 60)
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
