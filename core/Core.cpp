/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:22:17 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/24 15:25:01 by houmanso         ###   ########.fr       */
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
	int	kq;
	int	hook;
	size_t	i;
	size_t	size;
	std::vector<Server>::iterator	it;

	kq = kqueue();
	if (kq == -1)
		throw Fail(strerror(errno));
	size = servers.size();
	std::cout << "Core: running ..." << std::endl;
	checklist = new struct kevent[size];
	triggered = new struct kevent[size];
	for(i = 0; i < size; i++)
	{
		struct kevent ev;
		std::cout << "Core: run server" << std::endl;
		servers[i].setupServer();
		EV_SET(&checklist[i], servers[i].getSocketId(), EVFILT_READ, EV_ADD, 0,0,0);
	}
	while (1)
	{
		hook = kevent(kq, checklist, size, triggered, size, NULL);
		std::cout << "hook => " << hook << std::endl;
		for (i = 0; i < hook; i++)
		{
			
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
