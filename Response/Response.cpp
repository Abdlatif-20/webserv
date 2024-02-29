/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 05:13:21 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/29 20:18:55 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/* *************************** Constructors *************************** */

Response::Response()
{
	statusCode = 200;
	statusMessage = "OK";
	_request = Request();
}

Response::Response(Request request)
{
	statusCode = 200;
	statusMessage = "OK";
	_request = request;
}

Response::Response(const Response& obj)
{
	*this = obj;
}

Response& Response::operator=(const Response& obj)
{
	if (this != &obj)
	{
		statusCode = obj.statusCode;
		statusMessage = obj.statusMessage;
		_request = obj._request;
	}
	return (*this);
}

/* *************************** Destructors *************************** */

Response::~Response()
{
}

/* *************************** Member Functions *************************** */

void	Response::sendResponse(Request request)
{
	std::cout << "status for respons" << request.getStatus() << std::endl;
}
