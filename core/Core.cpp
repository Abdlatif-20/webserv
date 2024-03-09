/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:22:17 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/09 16:21:15 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Core.hpp"

Core::Core(void)
{

}

Core::Core(const Core &cpy)
{
	*this = cpy;
}

Core::Core(const Config &conf)
{
	ServersVector::iterator	it;

	config = conf;
	serversConf = conf.getServers();
	it = serversConf.begin();
	while (it != serversConf.end())
		servers.push_back(Server(*it++));
	std::sort(servers.begin(), servers.end());
	for (size_t i = 0; i < servers.size(); i++)
		std::cout << servers[i].getHostPort() << std::endl;
	std::cout << "================================================" << std::endl;
}

void	Core::run(void)
{
	size_t			binded;
	StringVector	ports;
	StringVector::iterator	p;
	std::vector<Server>::iterator	it;

	binded = 0;
	size = servers.size();
	for(int i = 0; i < size; i++)
	{
		p = std::find(ports.begin(), ports.end(), servers[i].getHostPort());
		if (p != ports.end())
			continue ;
		try
		{
			servers[i].setupServer();
			fcntl(servers[i].getSocketId(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
			ports.push_back(servers[i].getHost() + ":" + servers[i].getPort());
			binded++;
		}
		catch (const std::exception& e)
		{
			std::cerr << servers[i].getHostPort() << " : " << e.what() << std::endl;
		}
	}
	if (binded > 0)
		traceEvents();
	else
		std::cout << "info: no servers to bind !" << std::endl;
}

void	Core::traceEvents(void)
{
	int	fd;
	int	hooks;
	size_t	i;
	servers_it	it;

	std::cout << "Server is running ..." << std::endl;
	while (true)
	{
		for (it = servers.begin(); it !=  servers.end(); it++)
		{
			fd = accept(it->getSocketId(), NULL, 0);
			if (fd != -1)
			{
				clients[fd].setSockId(fd);
				clients[fd].setServerCTX(it->getServerCTX());
				checklist.push_back((pollfd){fd, POLLIN | POLLOUT | POLLHUP, 0});
				std::cout << it->getHostPort() << std::endl;
			}
			if (it != servers.end() && *it == *(it + 1))
				it++;
		}
		if (!checklist.size())
			continue ;
		hooks = poll(checklist.data(), checklist.size(), 0);
		for (i = 0; i < checklist.size(); i++)
		{
			if (checklist[i].revents & POLLIN)
			{
				clients[checklist[i].fd].recvRequest();
				if (clients[checklist[i].fd].hostIsDetected())
				{
					
				}
			}
			if (checklist[i].revents & POLLOUT && clients[checklist[i].fd].isRequestDone())
				clients[checklist[i].fd].sendResponse();
			if (checklist[i].revents & POLLHUP || clients[checklist[i].fd].isResponseDone())
			{
				clients.erase(checklist[i].fd);
				checklist.erase(checklist.begin() + i--);
			}
		}
	}
}

// bool Core::countainHost(const server &server)
// {
// 	return ;
// }

Core &Core::operator=(const Core &cpy)
{
	if (this != &cpy)
	{
		servers = cpy.servers;
		serversConf = cpy.serversConf;
	}
	return (*this);
}

Core::~Core(void)
{
}
