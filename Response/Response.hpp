/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 05:13:27 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/29 20:18:55 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

class Client;

class Response
{
	private:
		int statusCode;
		std::string statusMessage;
		Request _request;
	public:
		Response();
		Response(Request request);
		Response(const Response& obj);
		Response& operator=(const Response& obj);
		~Response();
		void sendResponse(Request request);
};
