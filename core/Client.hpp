/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:50:55 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/17 18:25:02 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

#define JJJ "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd"
#define	TIMEOUT 60

class Client
{
	private:
		ssize_t	len;

		int		sockId;
		char	buff[1024];
		bool	requestDone;
		bool	responseDone;
		time_t	last_update_time;

		std::string	resp;

		Request	request;
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
		bool	timeout(void) const;
		bool	isRequestDone(void) const;
		bool	isResponseDone(void) const;
		bool	hostIsDetected(void) const;

		void	selectServerCTX(void);
		void	setServersEnd(size_t it);
		void	setServersBegin(size_t it);

		const Request&	getRequest(void) const;
		const Response&	getResponse(void) const;

		time_t	getLastUpdateTime(void) const;

		Client&	operator=(const Client& cpy);

		~Client(void);
};

#endif
