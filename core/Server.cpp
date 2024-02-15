/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:55:56 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/15 19:17:13 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
}

Server::Server(const ServerContext & _serverCTX)
{
	serverCTX = _serverCTX;
}

Server::Server(const Server &cpy)
{
	*this = cpy;
}

std::string Server::getPORT_tmp(void) const
{
	return (serverCTX.getListen());
}

Server &Server::operator=(const Server &cpy)
{
	if (this != &cpy)
	{
		// ...
	}
	return (*this);
}

Server::~Server(void)
{
}
