/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/14 11:26:48 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Request.hpp"

/* *************************** Constructors *************************** */

Request::Request()
{
	_status = 200;
	_detectHost = 0;
	_bodyDone = false;
	_foundUri = false;
	_receivecount = 0;
	_contentLength = 0;
	_headersDone = false;
	_requestLineDone = false;
	requestInProgress = false;
	_requestIsWellFormed = false;
	_body = "";
	_params = "";
	headers = "";
	_configPath = "";
	_requestData = "";
	_boundaryName = "";
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
