/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:21:50 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/23 20:58:03 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
#define CORE_HPP

#include <climits>
#include <iostream>

#include <libc.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/socket.h>

#include "Config.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Client;

class Core
{
	private:
		long	size;
		Config	config;
		ServersVector	serversConf;
		std::map<int, Client>	clients;
		std::vector<struct pollfd>	checklist;

	public:
		static std::vector<Server>	servers;

		Core(void);
		Core(const Core& cpy);
		Core(Config& conf);

		void	run(void);
		void	traceEvents(void);

		// bool	countainHost(const server& server);
		Core&	operator=(const Core& cpy);

		~Core(void);
};

#endif
