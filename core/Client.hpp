/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:50:55 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/10 18:17:24 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client
{
	private:
		ssize_t	len;

		int		sockId;
		char	buff[1024];
		bool	requestDone;
		bool	responseDone;
		bool	serverSelected;

		Request	request;
		std::string	resp;
		Response	response;
		servers_it	serv_end;
		servers_it	serv_begin;
		ServerContext	serverCTX;

	public:
		Client(void);
		Client(int sock);
		Client(const Client& cpy);

		int		recvRequest(void);
		void	sendResponse(void);

		void	setSockId(int sock);
		void	setServerCTX(const ServerContext& serverCTX);

		bool	isRequestDone(void) const;
		bool	isResponseDone(void) const;
		bool	hostIsDetected(void) const;

		void	selectServerCTX(void);
		void	setServersEnd(servers_it it);
		void	setServersBegin(servers_it it);

		const Request&	getRequest(void) const;
		const Response&	getResponse(void) const;

		const servers_it	serversEnd(void) const;
		const servers_it	serversBegin(void) const;

		Client&	operator=(const Client& cpy);

		~Client(void);
};

#endif
