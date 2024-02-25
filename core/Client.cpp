/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:51:06 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/25 17:39:05 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
}

Client::Client(int sock)
{
	sockId = sock;
}

// Client::Client(const Server &serverCTX)
// {
// 	this->serverCTX = serverCTX;
// }

Client::Client(const Client &cpy)
{
	*this = cpy;
}

void	Client::recvRequest(void)
{
	int	len;
	char	buff[1024];

	len = recv(sockId, buff, 1023, 0);
	if (len <= -1)
		return ;
	buff[len] = '\0';
	ss << len;
	std::cout << buff;
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
		// serverCTX = cpy.serverCTX;
	}
	return (*this);
}

Client::~Client(void)
{
	close(sockId);
}
