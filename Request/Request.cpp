/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/14 14:37:48 by houmanso         ###   ########.fr       */
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
	this->requestLineDone = false;
	this->requestIscomplete = false;
	this->requestInProgress = false;
	this->remainingChunkLength = 0;
	this->_requestIsWellFormed = false;
	this->requestLineInProgress = false;
	this->_path = "";
	this->_body = "";
	this->headers = "";
	this->context = NULL;
	this->requestData = "";
	this->boundaryName = "";
	this->_chunkedName = "";
	this->requestLineData = "";
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

const String&	Request::getBody() const
{
	return (_body);
}

const std::map<String, String>&	Request::getHeaders() const
{
	return (_headers);
}

const std::map<String, String>&	Request::getRequestLine() const
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

const String Request::getHeaderByName(const String& name) const
{
	Utils::toLower((String&)name);
	try
	{
		return (_headers.at(name));
	}
	catch(const std::exception& e)
	{
		if (name == "connection")
			return ("keep-alive");
	}
	return ("");
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

bool	Request::hostIsDetected(void) const
{
	return (detectHost);
}

const String Request::getMethod() const
{
	try{
		return requestLine.at("method");
	}
	catch(const std::exception& e) {}
	return ("");
}

const String Request::getHost() const
{
	try{
		return (_headers.at("host"));
	}
	catch(const std::exception& e){}
	return ("");
}

Context* Request::getContext() const
{
	return context;
}

const std::string Request::getRequestPath() const
{
	try{
		return (requestLine.at("path"));
	}
	catch(const std::exception& e){}
	return ("");
}

/* *************************** methods *************************** */

void	Request::resetRequest()
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
	this->requestIscomplete = false;
	this->requestLineDone = false;
	this->requestInProgress = false;
	this->remainingChunkLength = 0;
	this->_requestIsWellFormed = false;
	this->requestLineInProgress = false;
	this->_path = "";
	this->_body = "";
	this->headers = "";
	this->context = NULL;
	this->requestData = "";
	this->boundaryName = "";
	this->_chunkedName = "";
	this->requestLine.clear();
	this->params.clear();
	this->headers.clear();
	this->requestData.clear();
	this->requestVector.clear();
}

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
