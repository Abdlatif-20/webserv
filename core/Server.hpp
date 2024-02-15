/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:23:57 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/15 19:15:40 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include "ServerContext.hpp"

class Server
{
	private:
		// std::string	port;
		// int	sockID;
		ServerContext	serverCTX;

	public:
		Server(void);
		Server(const ServerContext& _serverCTX);
		Server(const Server& cpy);

		std::string	getPORT_tmp(void) const;

		Server&	operator=(const Server& cpy);

		~Server(void);
};

#endif
