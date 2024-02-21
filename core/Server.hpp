/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:23:57 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/20 19:18:03 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Utils.hpp"
#include "ServerContext.hpp"

class Server
{
	private:
		int	sockID;
		sockaddr_in	addr;
		std::string	port;
		std::string	host;
		addrinfo	x;
		StringVector	serverNames;
		ServerContext	serverCTX;

	public:
		Server(void);
		Server(const Server& cpy);
		Server(const ServerContext& _serverCTX);

		Server&	operator=(const Server& cpy);

		~Server(void);
};

#endif
