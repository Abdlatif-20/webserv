/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/28 21:39:59 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Request.hpp"

std::vector<Server> Request::servers;

/* *************************** Constructors *************************** */

Request::Request()
{
	this->status = OK;
	this->tmpFile = -1;
	this->detectHost = 0;
	this->bodyDone = false;
	this->lastTime = std::time(NULL);
	this->foundUri = false;
	this->receivecount = 0;
	this->sizeBoundary = 0;
	this->contentLength = 0;
	this->multipart = false;
	this->_setLength = false;
	this->isComplete = false;
	this->remainingChunk = 0;
	this->headersDone = false;
	this->requestLineDone = false;
	this->remainingChunkLength = 0;
	this->_chunkedComplete = false;
	this->requestIscomplete = false;
	this->requestInProgress = false;
	this->_BoundaryComplete = false;
	this->_requestIsWellFormed = false;
	this->requestLineInProgress = false;
	this->_path.clear();
	this->_body.clear();
	this->_params.clear();
	this->headers.clear();
	this->requestData.clear();
	this->boundaryName.clear();
	this->_chunkedName.clear();
	this->_pathTmpFile.clear();
	this->requestVector.clear();
	this->requestLineData.clear();
	bzero(this->buffer, BUFFER_SIZE);
}

Request::Request(const Request& obj)
{
	*this = obj;
}

Request& Request::operator=(const Request& obj)
{
	if (this != &obj)
	{
		this->_body = obj._body;
		this->_path = obj._path;
		serv_end = obj.serv_end;
		this->status = obj.status;
		serv_begin = obj.serv_begin;
		this->_params = obj._params;
		this->headers = obj.headers;
		this->serverCTX = obj.serverCTX;
		this->locationCTX = obj.locationCTX;
		this->bodyDone = obj.bodyDone;
		this->foundUri = obj.foundUri;
		this->multipart = obj.multipart;
		this->_setLength = obj._setLength;
		this->isComplete = obj.isComplete;
		this->detectHost = obj.detectHost;
		this->requestLine = obj.requestLine;
		this->headersDone = obj.headersDone;
		this->requestData = obj.requestData;
		this->receivecount = obj.receivecount;
		this->_chunkedName = obj._chunkedName;
		this->sizeBoundary = obj.sizeBoundary;
		this->boundaryName = obj.boundaryName;
		this->contentLength = obj.contentLength;
		this->requestVector = obj.requestVector;
		this->remainingChunk = obj.remainingChunk;
		this->requestLineDone = obj.requestLineDone;
		this->_chunkedComplete = obj._chunkedComplete;
		this->requestIscomplete = obj.requestIscomplete;
		this->_BoundaryComplete = obj._BoundaryComplete;
		this->requestInProgress = obj.requestInProgress;
		this->_requestIsWellFormed = obj._requestIsWellFormed;
		this->remainingChunkLength = obj.remainingChunkLength;
		this->requestLineInProgress = obj.requestLineInProgress;
		this->requestLineData = obj.requestLineData;
		this->tmpFile = obj.tmpFile;
		this->lastTime = obj.lastTime;
		this->_pathTmpFile = obj._pathTmpFile;
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
		else
			return "";
	}
	return ("");
}

const ServerContext& Request::getServerCTX(void) const
{
	return serverCTX;
}

const LocationContext& Request::getLocationCtx(void) const
{
	return locationCTX;
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
	this->remainingChunk = 0;
	this->_setLength = false;
	this->isComplete = false;
	this->headersDone = false;
	this->requestLineDone = false;
	this->remainingChunkLength = 0;
	this->_chunkedComplete = false;
	this->_BoundaryComplete = false;
	this->requestIscomplete = false;
	this->requestInProgress = false;
	this->lastTime = std::time(NULL);
	this->_requestIsWellFormed = false;
	this->requestLineInProgress = false;
	this->_path.clear();
	this->_body.clear();
	this->_params.clear();
	this->headers.clear();
	this->headers.clear();
	this->requestData.clear();
	this->boundaryName.clear();
	this->_chunkedName.clear();
	this->requestLine.clear();
	this->requestData.clear();
	this->_pathTmpFile.clear();
	this->requestVector.clear();
	this->requestLineData.clear();
	close(this->tmpFile);
	this->tmpFile = -1;
	bzero(this->buffer, BUFFER_SIZE);
	
}

time_t	Request::getLastTime() const
{
	return (lastTime);
}

void	Request::selectServerContext(const String& host)
{
	servers_it it, end;
	StringVector::iterator	name;

	end = Request::servers.begin() + serv_end;
	it  = Request::servers.begin() + serv_begin;
	while (it != end)
	{
		StringVector hosts(it->getServerNames());
		name = std::find(hosts.begin(), hosts.end(), host);
		if (name != hosts.end())
		{
			serverCTX = it->getServerCTX();
			break;
		}
		it++;
	}
}

void	Request::setServerCTXEnd(size_t i)
{
	serv_end = i;
}

void	Request::setServerCTXBegin(size_t i)
{
	serv_begin = i;
}

//main function to parse the request
void	Request::parseRequest(const std::string& receivedRequest, ServerContext serverCTX)
{
	if (receivedRequest.empty())
		return;
	this->serverCTX = serverCTX;
	std::srand(time(NULL));
	if (!this->requestLineDone || !this->headersDone || !this->_requestIsWellFormed)
	{
		/* L body i9dar ikoun f GET wla delete 3la 7ssab CGI !!! */
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
	if (this->requestLine["method"] == "POST" && !this->bodyDone
		&& this->_requestIsWellFormed && this->status == 200 && foundUri)
	{
		setUploadingPath();
		if (status != 200)
			return;
		if (this->receivecount > 1)
		{
			if (receivedRequest.front() == CR && this->requestInProgress)
				return;
			this->_body = receivedRequest;
		}
		if (!this->_body.empty())
			parseBody();
		if (receivecount == 1)
			receivecount++;
	}
}
