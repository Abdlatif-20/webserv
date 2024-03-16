/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:50:55 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/16 00:07:12 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

#define JJJ "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd"

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
		ServerContext	serverCTX;

	public:
		Client(void);
		Client(int sock);
		Client(const Client& cpy);

		void	reset(void);
		ssize_t	recvRequest(void);
		void	sendResponse(void);

		void	setSockId(int sock);
		void	setServerCTX(const ServerContext& serverCTX);

		bool	isALive(void) const;
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
