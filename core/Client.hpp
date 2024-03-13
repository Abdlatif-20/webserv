/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:50:55 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/12 17:05:48 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Core.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Core;

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
		size_t	serv_end;
		std::string	resp;
		size_t	serv_begin;
		Response	response;
		ServerContext	serverCTX;

	public:
		Client(void);
		Client(int sock);
		Client(const Client& cpy);

		ssize_t	recvRequest(void);
		void	sendResponse(void);

		void	setSockId(int sock);
		void	setServerCTX(const ServerContext& serverCTX);

		bool	isRequestDone(void) const;
		bool	isResponseDone(void) const;
		bool	hostIsDetected(void) const;

		void	selectServerCTX(void);
		void	setServersEnd(size_t it);
		void	setServersBegin(size_t it);

		const Request&	getRequest(void) const;
		const Response&	getResponse(void) const;

		size_t	serversEnd(void) const;
		size_t	serversBegin(void) const;

		Client&	operator=(const Client& cpy);

		~Client(void);
};

#endif
