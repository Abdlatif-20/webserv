/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:57:14 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/04 15:50:43 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Server
{
	private:
		int	socketID;
		sockaddr_in	addr;
	public:
		Server(void);
		Server(const Server& cpy);

		Server	&operator=(const Server& cpy);

		void	run(void);
		void	setupAddr(void);

		~Server(void);
};

#endif

