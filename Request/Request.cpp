/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/20 10:28:49 by aben-nei         ###   ########.fr       */
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
	_isComplete = false;
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

const std::string&	Request::getBody()
{
	return (_body);
}

const std::map<std::string, std::string>&	Request::getHeaders()
{
	return (_headers);
}

const std::map<std::string, std::string>&	Request::getRequestLine()
{
	return (_requestLine);
}

int	Request::getStatus()
{
	return (_status);
}

const bool& Request::getRequestIsWellFormed()
{
	return (_requestIsWellFormed);
}

const bool& Request::getBodyDone()
{
	return (_bodyDone);
}

const bool& Request::getHeadersDone()
{
	return (_headersDone);
}

const bool& Request::getRequestLineDone()
{
	return (_requestLineDone);
}

const bool& Request::getFoundUri()
{
	return (_foundUri);
}

/* *************************** methods *************************** */

//main function to parse the request
void	Request::parseRequest(const std::string& receivedRequest, char *configPath)
{
	// std::cout <<"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	// std::cout << "Request:\n" << receivedRequest << std::endl;
	// std::cout <<"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
	_configPath = configPath;

	std::srand(time(0));
	if (!_requestLineDone || !_headersDone || !_requestIsWellFormed)
	{
		if (takingRequests(receivedRequest))
		{
			if (_detectHost > 1)
			{
				_status = BadRequest;
				throw InvalidRequest("Duplicate Host");
			}
			return;
		}
	}
	if (_requestLine["method"] == "POST" && !_bodyDone)
	{
		if (_receivecount > 1 && !requestInProgress)
			_body = receivedRequest;
		if (_receivecount > 1 && requestInProgress)
		{
			if (receivedRequest == CRLF)
				return;
			_body = receivedRequest;
		}
		if (!_body.empty())
			parseBody();
		// std::cout << "-------- Body --------" << std::endl;
		// std::cout << _body << std::endl;
		// std::cout << "---------------------------" << std::endl;
	}
	std::cout << "-------- RequestLine --------" << std::endl;
	Utils::printMap(_requestLine);
	std::cout << "-------- Headers --------" << std::endl;
	Utils::printMap(_headers);
	// std::cout << "Request is well formed" << std::endl;
}
