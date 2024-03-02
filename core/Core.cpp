/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:22:17 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/02 14:03:10 by houmanso         ###   ########.fr       */
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
	{
		servers.push_back(Server(*it));
		it++;
	}
}

void	Core::run(void)
{
	StringVector	ports;
	StringVector::iterator	p;
	std::vector<Server>::iterator	it;

	size = servers.size();
	for(int i = 0; i < size; i++)
	{
		p = std::find(ports.begin(), ports.end(), servers[i].getPort());
		if (p == ports.end())
		{
			try
			{
				servers[i].setupServer();
				ports.push_back(servers[i].getPort());
				fcntl(servers[i].getSocketId(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}
	kq = kqueue();
	if (kq == -1)
		throw Fail(strerror(errno));
	traceEvents();
}

void	Core::traceEvents(void)
{
	int	fd;
	int	hooks;
	size_t	i;
	servers_it	it;

	std::cout << "Server is running ..." << std::endl;
	std::cout << checklist.size() << std::endl;
	while (true)
	{
		for (it = servers.begin(); it !=  servers.end(); it++)
		{
			fd = accept(it->getSocketId(), NULL, 0);
			if (fd != -1)
			{
				clients[fd].setSockId(fd);
				clients[fd].setConfig(config);
				checklist.push_back((pollfd){fd, POLLIN|POLLOUT, 0});
			}
		}
		if (!checklist.size())
			continue ;
		hooks = poll(checklist.data(), checklist.size(), 0);
		for (i = 0; i < checklist.size(); i++)
		{
			if (checklist[i].revents & POLLHUP)
				clients.erase(checklist[i].fd);
			if (checklist[i].revents & POLLIN)
				clients[checklist[i].fd].recvRequest();
			if (checklist[i].revents & POLLOUT && clients[checklist[i].fd].isRequestDone())
				clients[checklist[i].fd].sendResponse();
		}
	}
}

Core	&Core::operator=(const Core &cpy)
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
	close(kq);
}
