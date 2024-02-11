/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/11 17:29:11 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Request.hpp"

/* *************************** Constructors *************************** */
Request::Request()
{
	_status = 200;
	_bodyDone = false;
	_receivecount = 0;
	_contentLength = 0;
	_headersDone = false;
	_requestLineDone = false;
	_requestIsWellFormed = false;
	_foundUri = false;
}

Request::~Request()
{
}

/* *************************** getters *************************** */

std::string	Request::getBody()
{
	return (_body);
}

std::map<std::string, std::string>	Request::getHeaders()
{
	return (_headers);
}

std::map<std::string, std::string>	Request::getRequestLine()
{
	return (_requestLine);
}

int	Request::getStatus()
{
	return (_status);
}
