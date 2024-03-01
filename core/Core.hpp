/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:21:50 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/01 11:52:34 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
#define CORE_HPP

#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <libc.h>
#include <netdb.h>
#include <climits>
#include <netdb.h>
#include "Config.hpp"
#include "Server.hpp"
#include "Client.hpp"

#define JJJ "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd"


class Core
{
	private:
		int	kq;
		long	size;
		Config	config;
		ServersVector	serversConf;
		std::vector<Server>	servers;
		std::map<int, Client>	clients;
		struct kevent	triggered[OPEN_MAX];
		struct kevent	checklist[OPEN_MAX];

		typedef std::vector<Server>::iterator	v_it;

	public:
		Core(void);
		Core(const Core& cpy);
		Core(const Config& conf);

		void	run(void);
		void	traceEvents(void);

		Core&	operator=(const Core& cpy);

		~Core(void);
};

#endif
