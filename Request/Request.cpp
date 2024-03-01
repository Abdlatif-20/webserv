/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/29 16:33:16 by mel-yous         ###   ########.fr       */
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
	_requestIsDone = false;
	_requestLineDone = false;
	requestInProgress = false;
	_requestIsWellFormed = false;
	_body = "";
	_params = "";
	headers = "";
	_requestData = "";
	_boundaryName = "";
	_requestIsDone = false;
}

Request::Request(const Request& obj)
{
	*this = obj;
}

Request& Request::operator=(const Request& obj)
{
	if (this != &obj)
	{
		_status = obj._status;
		_detectHost = obj._detectHost;
		_bodyDone = obj._bodyDone;
		_foundUri = obj._foundUri;
		_receivecount = obj._receivecount;
		_contentLength = obj._contentLength;
		_isComplete = obj._isComplete;
		_headersDone = obj._headersDone;
		_requestLineDone = obj._requestLineDone;
		requestInProgress = obj.requestInProgress;
		_requestIsWellFormed = obj._requestIsWellFormed;
		_config = obj._config;
		_headers = obj._headers;
		_requestLine = obj._requestLine;
		_requestVector = obj._requestVector;
		_body = obj._body;
		_params = obj._params;
		headers = obj.headers;
		_requestData = obj._requestData;
		_boundaryName = obj._boundaryName;
		_requestIsDone = obj._requestIsDone;
	}
	return (*this);
}

Request::~Request()
{
}

/* *************************** getters *************************** */

const std::string&	Request::getBody() const
{
	return (_body);
}

const std::map<std::string, std::string>&	Request::getHeaders() const
{
	return (_headers);
}

const std::map<std::string, std::string>&	Request::getRequestLine() const
{
	return (_requestLine);
}

const int&	Request::getStatus() const
{
	return (_status);
}

void	Request::setStatus(int status)
{
	_status = status;
}

const bool& Request::getRequestIsWellFormed() const
{
	return (_requestIsWellFormed);
}

const bool& Request::getBodyDone() const
{
	return (_bodyDone);
}

const bool& Request::getHeadersDone() const
{
	return (_headersDone);
}

const std::string& Request::getHeaderByName(const std::string& name) const
{
	return (_headers.at(name));
}

const bool& Request::getRequestLineDone() const
{
	return (_requestLineDone);
}

const bool& Request::getFoundUri() const
{
	return (_foundUri);
}

const std::string& Request::getMethod() const
{
	return _requestLine.at("method");
}

const std::string& Request::getHost() const
{
	return _headers.at("host");
}

/* *************************** methods *************************** */

//main function to parse the request
void	Request::parseRequest(const std::string& receivedRequest, const Config& config)
{
	_config = config;
	std::srand(time(0));
	if (!_requestLineDone || !_headersDone || !_requestIsWellFormed)
	{
		if (takingRequests(receivedRequest))
		{
			if (_detectHost > 1)
				_status = BadRequest;
			return;
		}
	}
	if (_requestLine["method"] == "POST" && !_bodyDone)
	{
		if (_receivecount > 1)
		{
			if (receivedRequest.front() == CR && requestInProgress)
				return;
			_body = receivedRequest;
		}
		if (!_body.empty())
			parseBody();
	}
	_requestIsDone = true;
}
