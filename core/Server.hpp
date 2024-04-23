/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:23:57 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/10 11:41:33 by houmanso         ###   ########.fr       */
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
		std::string	port;
		std::string	host;
		std::string	host_port;
		struct addrinfo	*res;
		ServerContext	serverCTX;
		StringVector	serverNames;
	public:
		Server(void);
		Server(const Server& cpy);
		Server(const ServerContext& _serverCTX);

		void	setupServer(void);

		int		getSocketId(void) const;

		const std::string&	getHost(void) const;
		const std::string&	getPort(void) const;
		const std::string&	getHostPort(void) const;

		const StringVector&	getServerNames(void) const;
		const ServerContext&	getServerCTX(void) const;

		Server&	operator=(const Server& cpy);
		bool	operator<(const Server& cpy) const;
		bool	operator>(const Server& cpy) const;
		bool	operator>=(const Server& cpy) const;
		bool	operator<=(const Server& cpy) const;
		bool	operator==(const Server& cpy) const;

		~Server(void);
};

#endif
