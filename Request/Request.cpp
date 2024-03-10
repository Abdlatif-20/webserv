/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/09 13:14:34 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Request.hpp"

/* *************************** Constructors *************************** */

Request::Request()
{
	status = OK;
	detectHost = 0;
	bodyDone = false;
	foundUri = false;
	receivecount = 0;
	sizeBoundary = 0;
	contentLength = 0;
	multipart = false;
	_setLength = false;
	isComplete = false;
	headersDone = false;
	requestIscomplete = false;
	requestLineDone = false;
	requestInProgress = false;
	remainingChunkLength = 0;
	_requestIsWellFormed = false;
	_path = "";
	_body = "";
	headers = "";
	requestData = "";
	boundaryName = "";
	_chunkedName = "";
	context = NULL;
}

Request::Request(const Request& obj)
{
	*this = obj;
}

Request& Request::operator=(const Request& obj)
{
	if (this != &obj)
	{
		_body = obj._body;
		_path = obj._path;
		status = obj.status;
		params = obj.params;
		headers = obj.headers;
		headers = obj.headers;
		bodyDone = obj.bodyDone;
		context = obj.context;
		foundUri = obj.foundUri;
		multipart = obj.multipart;
		_setLength = obj._setLength;
		isComplete = obj.isComplete;
		detectHost = obj.detectHost;
		requestLine = obj.requestLine;
		headersDone = obj.headersDone;
		requestData = obj.requestData;
		receivecount = obj.receivecount;
		_chunkedName = obj._chunkedName;
		sizeBoundary = obj.sizeBoundary;
		boundaryName = obj.boundaryName;
		contentLength = obj.contentLength;
		requestVector = obj.requestVector;
		requestLineDone = obj.requestLineDone;
		requestIscomplete = obj.requestIscomplete;
		requestInProgress = obj.requestInProgress;
		_requestIsWellFormed = obj._requestIsWellFormed;
		remainingChunkLength = obj.remainingChunkLength;
		locationCtx = obj.locationCtx;
		context = obj.context;
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
	return (requestLine);
}

const int&	Request::getStatus() const
{
	return (status);
}

void	Request::setStatus(int status)
{
	this->status = status;
}

const bool& Request::getRequestIsWellFormed() const
{
	return (_requestIsWellFormed);
}

const bool& Request::getBodyDone() const
{
	return (bodyDone);
}

const bool& Request::getHeadersDone() const
{
	return (headersDone);
}

const std::string& Request::getHeaderByName(const std::string& name) const
{
	return (_headers.at(name));
}

const bool& Request::getRequestLineDone() const
{
	return (requestLineDone);
}

const bool& Request::getFoundUri() const
{
	return (foundUri);
}

bool	Request::isDone(void) const
{
	return (requestIscomplete);
}

const std::string &Request::getMethod() const
{
	return requestLine.at("method");
}

const std::string& Request::getHost() const
{
	return _headers.at("host");
}

Context* Request::getContext() const
{
	return context;
}

const std::string& Request::getRequestPath() const
{
	return requestLine.at("path");
}

/* *************************** methods *************************** */

//main function to parse the request
void	Request::parseRequest(const std::string& receivedRequest, Context* serverCTX)
{
	if (receivedRequest.empty())
		return;
	if (!context)
		this->context = serverCTX;
	std::srand(time(0));
	if (!this->requestLineDone || !this->headersDone || !this->_requestIsWellFormed)
	{
		if (takingRequests(receivedRequest))
		{
			if (detectHost > 1)
			{
				this->status = BadRequest;
				requestIscomplete = true;
			}
			return;
		}
	}
	if (this->requestLine["method"] != "POST" && this->_requestIsWellFormed && this->headersDone && this->requestLineDone)
		this->requestIscomplete = true;
	if (this->requestLine["method"] == "POST" && !this->bodyDone && this->_requestIsWellFormed && this->status == 200)
	{
		setUploadingPath();
		if (this->receivecount > 1)
		{
			if (receivedRequest.front() == CR && this->requestInProgress)
				return;
			this->_body = receivedRequest;
		}
		if (!this->_body.empty())
			parseBody();
	}
}
