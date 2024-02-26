/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 05:13:27 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/26 06:16:38 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"

class Client;

class Response
{
	private:
		int _statusCode;
		std::string _statusMessage;
		Request _request;
	public:
		Response();
		Response(Request request);
		Response(const Response& obj);
		Response& operator=(const Response& obj);
		~Response();
		void sendResponse(Request request);
};
