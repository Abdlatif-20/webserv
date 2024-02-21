/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:21:50 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/18 17:24:45 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
#define CORE_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Config.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Core
{
	private:
		ServersVector	serversConf;
		std::vector<Server>	servers;
		std::vector<Client>	clients;

	public:
		Core(void);
		Core(const Core& cpy);
		Core(const Config& conf);

		Core&	operator=(const Core& cpy);

		~Core(void);
};

#endif
