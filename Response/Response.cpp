/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 05:13:21 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/26 06:16:31 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/* *************************** Constructors *************************** */

Response::Response()
{
	_statusCode = 200;
	_statusMessage = "OK";
	_request = Request();
}

Response::Response(Request request)
{
	_statusCode = 200;
	_statusMessage = "OK";
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
		_statusCode = obj._statusCode;
		_statusMessage = obj._statusMessage;
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
