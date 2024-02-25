/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:22:17 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/25 17:38:12 by houmanso         ###   ########.fr       */
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
	std::vector<Server>::iterator	it;

	std::cout << "Core: running ..." << std::endl;
	size = servers.size();
	for(int i = 0; i < size; i++)
	{
		servers[i].setupServer();
		fcntl(servers[i].getSocketId(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
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
	struct timespec	t = {5, 0};

	n = 0;
	while (1)
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "accepting ..." << std::endl;
		for (it = servers.begin(); it !=  servers.end() && n < OPEN_MAX; it++)
		{
			fd = accept(it->getSocketId(), NULL, 0);
			if (fd != -1)
			{
				std::cout << "accepted : "<< fd << std::endl;
				clients[fd] = Client();
				clients[fd].setSockId(fd);
				EV_SET(&checklist[n++], fd, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_DELETE, 0, 0, 0);
			}
		}
		std::cout << "check for some events!" << std::endl;
		hooks = kevent(kq, checklist, n, triggered, n, &t);
		std::cout << "events : "<< hooks << std::endl;
		for (i = 0; i < hooks; i++)
		{
			if (clients.find(triggered[i].ident) != clients.end())
			{
				clients[triggered[i].ident].recvRequest();
				std::cout << "client detected " << triggered[i].ident << std::endl;
				if (triggered[i].fflags & NOTE_WRITE)
				{
					// triggered[i].fflags &= ~NOTE_WRITE;
				}
			}
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
}
