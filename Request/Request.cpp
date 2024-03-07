/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/05 04:34:12 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Request.hpp"

/* *************************** Constructors *************************** */

Request::Request()
{
	this->status = OK;
	this->detectHost = 0;
	this->bodyDone = false;
	this->foundUri = false;
	this->receivecount = 0;
	this->sizeBoundary = 0;
	this->contentLength = 0;
	this->multipart = false;
	this->_setLength = false;
	this->isComplete = false;
	this->headersDone = false;
	this->_requestIsDone = false;
	this->requestLineDone = false;
	this->requestInProgress = false;
	this->remainingChunkLength = 0;
	this->_requestIsWellFormed = false;
	this->_path = "";
	this->_body = "";
	this->headers = "";
	this->requestData = "";
	this->boundaryName = "";
	this->config = Config();
}

Request::Request(const Request& obj)
{
	*this = obj;
}

Request& Request::operator=(const Request& obj)
{
	if (this != &obj)
	{
		this->status = obj.status;
		this->detectHost = obj.detectHost;
		this->bodyDone = obj.bodyDone;
		this->foundUri = obj.foundUri;
		this->receivecount = obj.receivecount;
		this->contentLength = obj.contentLength;
		this->isComplete = obj.isComplete;
		this->headersDone = obj.headersDone;
		this->requestLineDone = obj.requestLineDone;
		this->requestInProgress = obj.requestInProgress;
		this->_requestIsWellFormed = obj._requestIsWellFormed;
		this->config = obj.config;
		this->headers = obj.headers;
		this->requestLine = obj.requestLine;
		this->requestVector = obj.requestVector;
		this->_body = obj._body;
		this->params = obj.params;
		this->headers = obj.headers;
		this->requestData = obj.requestData;
		this->boundaryName = obj.boundaryName;
		this->_requestIsDone = obj._requestIsDone;
		this->_setLength = obj._setLength;
		this->sizeBoundary = obj.sizeBoundary;
		this->remainingChunkLength = obj.remainingChunkLength;
		this->multipart = obj.multipart;
		this->_path = obj._path;
		this->_chunkedName = obj._chunkedName;
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

const std::string& Request::getMethod() const
{
	return requestLine.at("method");
}

const std::string& Request::getHost() const
{
	return _headers.at("host");
}

/* *************************** methods *************************** */

//main function to parse the request
void	Request::parseRequest(const std::string& receivedRequest, const Config& config)
{
	this->config = config;
	std::srand(time(0));
	setUploadingPath();
	if (!this->requestLineDone || !this->headersDone || !this->_requestIsWellFormed)
	{
		if (takingRequests(receivedRequest))
		{
			if (detectHost > 1)
				this->status = BadRequest;
			return;
		}
	}
	if (this->requestLine["method"] == "POST" && !this->bodyDone && this->_requestIsWellFormed && this->status == 200)
	{
		if (this->receivecount > 1)
		{
			if (receivedRequest.front() == CR && this->requestInProgress)
				return;
			this->_body = receivedRequest;
		}
		if (!this->_body.empty())
			parseBody();
	}
	if ((this->requestLine["method"] == "GET" && this->_requestIsWellFormed)
		|| (this->requestLine["method"] == "POST" && this->_requestIsWellFormed && this->bodyDone))
		this->_requestIsDone = true;
}
