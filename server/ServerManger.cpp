/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManger.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:25:30 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/14 13:26:17 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManger.hpp"

ServerManger::ServerManger(void)
{
	// nothing
}

ServerManger::ServerManger(const Config &conf)
{
	config = conf;
}

ServerManger::ServerManger(const ServerManger &cpy)
{
	*this = cpy;
}

ServerManger	&ServerManger::operator=(const ServerManger &cpy)
{
	if (this != &cpy)
	{
		config = cpy.config;
		servers = cpy.servers;
	}
	return (*this);
}

ServerManger::~ServerManger(void)
{
}
