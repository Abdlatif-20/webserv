/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManger.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:21:59 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/14 15:17:58 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANGER_HPP
#define SERVERMANGER_HPP

#include <iostream>
#include "Config.hpp"
#include "Server.hpp"

class ServerManger
{
	private:
		Config	config;
		std::vector<Server>	servers;

	public:
		ServerManger(void);
		ServerManger(const Config &conf);
		ServerManger(const ServerManger& cpy);

		ServerManger&	operator=(const ServerManger& cpy);

		~ServerManger(void);
};

#endif
