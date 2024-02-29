/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/29 20:39:39 by aben-nei         ###   ########.fr       */
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
	_requestIsDone = false;
	requestLineDone = false;
	requestInProgress = false;
	remainingChunkLength = 0;
	_requestIsWellFormed = false;

	_body = "";
	headers = "";
	requestData = "";
	_chunkedName = "";
	this->boundaryName = "";
	config = Config();
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
		this->_chunkedName = obj._chunkedName;
		this->_requestIsDone = obj._requestIsDone;
		this->_setLength = obj._setLength;
		this->sizeBoundary = obj.sizeBoundary;
		this->remainingChunkLength = obj.remainingChunkLength;
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

/* *************************** methods *************************** */

//main function to parse the request
void	Request::parseRequest(const std::string& receivedRequest, const Config& config)
{
	this->config = config;
	std::srand(time(0));
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
	// std::cout << "status: " << status << std::endl;
	// std::cout <<"_body: " << _body << std::endl;
		if (!this->_body.empty())
			parseBody();
	}
	if ((this->requestLine["method"] == "GET" && this->_requestIsWellFormed)
		|| (this->requestLine["method"] == "POST" && this->_requestIsWellFormed && this->bodyDone))
		this->_requestIsDone = true;
	std::cout <<"--------------------------------------"<< std::endl;
	// std::cout << "size boundary: " << sizeBoundary << std::endl;
	// std::cout <<"startBoundary: " << _Boundary << std::endl;
	Utils::printMap(_headers);
	std::cout <<"--------------------------------------"<< std::endl;
	// Utils::printMap(requestLine);
}
