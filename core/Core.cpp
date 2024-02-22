/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:22:17 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/21 16:36:16 by houmanso         ###   ########.fr       */
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
	it = servers.begin();
	while (it != servers.end())
	{
		std::cout << "Core: run server" << std::endl;
		it->setupServer();
		it++;
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
