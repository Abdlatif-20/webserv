/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:22:17 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/15 17:44:56 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core.hpp"

Core::Core(void)
{
}

Core::Core(const Core __unused &cpy)
{
}

Core::Core(const Config &conf)
{
	ServersVector::iterator	it;

	serversConf = conf.getServers();
	it = serversConf.begin();
	while (it != serversConf.end())
	{
		// StringVector::const_iterator l = it->getDirectiveByKey("listen");
		// if (l != (it->getDirectives().cend() - 1)
		// 	std::cout <<  << std::endl;
		it++;
	}
}

Core	&Core::operator=(const Core &cpy)
{
	if (this != &cpy)
	{
		// .......
	}
	return (*this);
}

Core::~Core(void)
{
}
