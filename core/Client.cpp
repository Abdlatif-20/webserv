/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/10 18:21:53 by houmanso         ###   ########.fr       */
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

int	Client::recvRequest(void)
{
	std::memset(buff, 0, sizeof(buff));
	len = recv(sockId, buff, 1023, 0);
	if (len > 0)
	{
		request.parseRequest(std::string(buff, len), serverCTX);
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
	if (requestDone)
	{
		serverSelected = false;
		response.setServerCtx(serverCTX);
		response.setRequest(request);
		resp = response.generateResponse();
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
	servers_it	it(serv_begin);
	StringVector::iterator	name;

	try
	{
		host = request.getHost();
	}
	catch(...){
		return ;
	}
	while (it != serv_end)
	{
		StringVector hosts(it->getServerNames());
		name = std::find(hosts.begin(), hosts.end(), host);
		if (name != hosts.end())
		{
			serverCTX = it->getServerCTX();
			std::cout << "host => " << serverCTX.getServerName()[0] << std::endl;
			serverSelected = true;
			break;
		}
		it++;
	}
}

void Client::setServersEnd(servers_it it)
{
	serv_end = it;
}

void Client::setServersBegin(servers_it it)
{
	serv_begin = it;
}

const servers_it Client::serversEnd(void) const
{
	return (serv_end);
}

const servers_it Client::serversBegin(void) const
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
