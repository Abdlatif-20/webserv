/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:22:17 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/29 20:58:16 by houmanso         ###   ########.fr       */
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
	int		i;
	int		n;
	int		fd;
	v_it	it;
	int		hooks;
	Client	c;

	n = 0;
	while (1)
	{
		for (it = servers.begin(); it !=  servers.end() && n < OPEN_MAX; it++)
		{
			fd = accept(it->getSocketId(), NULL, 0);
			if (fd != -1)
			{
				(clients[fd] = c).setSockId(fd);
				EV_SET(&checklist[n++], fd, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_DELETE, 0, 0, 0);
			}
		}
		hooks = kevent(kq, checklist, n, triggered, n, 0);
		for (i = 0; i < hooks; i++)
		{
			clients[triggered[i].ident].recvRequest();
			clients[triggered[i].ident].sendResponse();
			clients.erase(triggered[i].ident);
			n--;
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
