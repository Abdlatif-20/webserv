/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:23:57 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/21 16:30:08 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <libc.h>
#include <netdb.h>
#include "Utils.hpp"
#include "ServerContext.hpp"

class Server
{
	private:
		int	sockID;
		sockaddr_in	addr;
		std::string	port;
		std::string	host;
		StringVector	serverNames;
		ServerContext	serverCTX;

	public:
		Server(void);
		Server(const Server& cpy);
		Server(const ServerContext& _serverCTX);

		void	setupServer(void);

		Server&	operator=(const Server& cpy);

		~Server(void);
};

#endif
