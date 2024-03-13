/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/13 14:06:56 by aben-nei         ###   ########.fr       */
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

ssize_t	Client::recvRequest(void)
{
	std::memset(buff, 0, sizeof(buff));
	len = recv(sockId, buff, 1023, 0);
	if (len > 0)
	{
		request.parseRequest(std::string(buff, len), &serverCTX);
		requestDone = request.isDone();
		if (!serverSelected)
			selectServerCTX();
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
			std::cout << response.getHeaders() << std::endl;
			response.setHeadersSent(true);
		}
		responseDone = response.responseIsDone();
		send(sockId, response.getBody().c_str(), response.getBody().size(), 0);
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

bool Client::isRequestDone(void) const
{
	return (requestDone);
}

bool	Client::isResponseDone(void) const
{
	return (responseDone);
}

void Client::selectServerCTX(void)
{
	std::string	host;
	servers_it it, end;
	StringVector::iterator	name;

	end = Core::servers.begin() + serv_end;
	it  = Core::servers.begin() + serv_begin;
	try
	{
		host = request.getHost();
	}
	catch(...){
		return ;
	}
	while (it != end)
	{
		StringVector hosts(it->getServerNames());
		name = std::find(hosts.begin(), hosts.end(), host);
		if (name != hosts.end())
		{
			serverCTX = it->getServerCTX();
			serverSelected = true;
			break;
		}
		it++;
	}
}

void Client::setServersEnd(size_t it)
{
	serv_end = it;
}

void Client::setServersBegin(size_t it)
{
	serv_begin = it;
}

size_t Client::serversEnd(void) const
{
	return (serv_end);
}

size_t Client::serversBegin(void) const
{
	return (serv_begin);
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
		serv_end = cpy.serv_end;
		serverCTX = cpy.serverCTX;
		serv_begin = cpy.serv_begin;
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
