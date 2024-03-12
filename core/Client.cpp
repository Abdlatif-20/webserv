/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/12 17:16:00 by houmanso         ###   ########.fr       */
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
	if (requestDone)
	{
		serverSelected = false;
		response.setRequest(&request);
		response.setServerCtx(serverCTX);
		requestDone = false;
		responseDone = true;
		// requestDone = false;
		// responseDone = false;
		// send(sockId, "HTTP/1.1 200 OK\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd", 87, 0);
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
