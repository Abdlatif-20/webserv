/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:23:29 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/10 20:48:52 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to set the path to the upload store
void	Request::setUploadingPath()
{
	this->_path = serverCTX.getUploadStore();
	this->_path += "/";
	if (!directoryExists(this->_path.c_str()))
		mkdir(this->_path.c_str(), 0777);
}

//function to Check if transfer encoding is chunked Or not
void	Request::parseTransferEncoding()
{
	if (_headers["transfer-encoding"] != "chunked")
		return (this->status = NotImplemented, requestIscomplete = true, void());
}

void	Request::isMethodAllowedInLocation()
{
	Vector allowedMethods = locationCtx.getAllowedMethods();
	if (std::find(allowedMethods.begin(), allowedMethods.end(), requestLine["method"]) == allowedMethods.end())
		return (std::cout << "Method not allowed in location" << std::endl, this->status = MethodNotAllowed, requestIscomplete = true, void());
	std::cout << "Method is allowed in location" << std::endl;
}

//function to check if the request Line is well formed And Set the status To true If all is well
void	Request::requestIsWellFormed()
{
	if (this->requestLine["path"].size() > 2048)
		return (this->status = RequestURITooLong, requestIscomplete = true, void());
	if (this->requestLine["path"].find_first_not_of(ALLOWED_CHARACTERS) != String::npos)
		return (this->status = BadRequest, requestIscomplete = true, void());
	if (this->requestLine["method"] == "POST")
	{
		if (_headers.find("content-type") != _headers.end()
				&& _headers.find("transfer-encoding") == _headers.end())
			parseContentType();
		else if (_headers.find("transfer-encoding") != _headers.end())
			parseTransferEncoding();
		else if (_headers.find("content-length") != _headers.end())
			parseContentLength();
		else
			return (this->status = BadRequest, requestIscomplete = true, void());
	}
	_requestIsWellFormed = true;
	isMethodAllowedInLocation();
}

//function to find the uri in the config file and set the status to true if found
void	Request::findUri()
{
	String uri = requestLine["path"];
	LocationContext _locationCtx = serverCTX.matchLocation(uri);
	if (_locationCtx.getPrefix() != "")
	{
		this->locationCtx = _locationCtx;
		std::cout <<"Matched location: " << _locationCtx.getPrefix() << std::endl;
		foundUri = true;
		return;
	}
	status = NotFound;
	requestIscomplete = true;
}

//function to separate the request line And the headers from the request
void	Request::separateRequest(String receivedRequest)
{
	size_t pos = receivedRequest.find("\r\n\r\n");
	if (pos != String::npos)
	{
		this->headers = receivedRequest.substr(0, pos + 4);
		this->_body = receivedRequest.substr(pos + 4);
	}
	else
	{
		this->status = BadRequest;
		requestIscomplete = true;
	}
}

void	Request::parseUri()
{
	if (this->requestLine["path"].find("%") != String::npos)
		Utils::decodeUri(this->requestLine["path"]);
}

//function to parse the Body of the request if method is POST
void	Request::parseBody()
{
	if (this->requestLine["method"] == "POST"
			&& _headers.find("transfer-encoding") != _headers.end()
			&& _headers.find("content-type") == _headers.end())
			parseChunkedBody();
	else if (this->requestLine["method"] == "POST"
		&& _headers.find("content-type") != _headers.end()
		&& _headers.find("transfer-encoding") == _headers.end())
			parseBoundary();
	else if (this->requestLine["method"] == "POST"
			&& _headers.find("content-length") != _headers.end())
				ContentLength();
}

//function to parse the request line and fill it to the map and return 1 if the request line is separated
int	Request::parseRequestLine(const String& requestLine)
{
	if (!requestLine.size())
		return 0;
	if (requestLine.find("\r\n\r\n") == String::npos)
	{
		if (requestLine.find("\r\n") == String::npos)
			return (requestLineInProgress = true, requestLineData += requestLine, 1);
		if (requestLineInProgress)
		{
			requestLineData += requestLine;
			this->requestVector = Utils::splitRequest(requestLineData, CRLF);
			this->requestInProgress = true;
			requestLineInProgress = false;
		}
		else
		{
			this->requestVector = Utils::splitRequest(requestLine, CRLF);
			this->requestInProgress = true;
		}
		return (fillRequestLine(this->requestVector[0]), 1);
	}
	return (this->requestVector = Utils::splitRequest(requestLine,
			CRLF), fillRequestLine(this->requestVector[0]),0);
}

//check if the request line or host is duplicated
int	Request::checkDuplicate(const String& receivedRequest)
{
	if (requestInProgress)
	{
		if (receivedRequest == CRLF && !requestLineInProgress)
			return 0;
		size_t pos = receivedRequest.find(" ");
		if (pos != String::npos)
		{
			String value = receivedRequest.substr(0, pos);
			if (value.find("GET") != String::npos
				|| value.find("POST") != String::npos
				|| value.find("DELETE") != String::npos
				|| value.find("HEAD") != String::npos
				|| value.find("PUT") != String::npos
				|| value.find("CONNECT") != String::npos
				|| value.find("OPTIONS") != String::npos
				|| value.find("TRACE") != String::npos)
					return (this->status = BadRequest, requestIscomplete = true, 0);
		}
		if (receivedRequest.find("host") != String::npos)
			detectHost++;
		if (receivedRequest.find("\r\n") == String::npos)
			return (requestData += receivedRequest, receivecount++, requestLineInProgress = true, 1);
		if (requestLineInProgress)
			return (requestData += receivedRequest, requestLineInProgress = false, 1);
		return (requestData += receivedRequest, receivecount++, 1);
	}
	return (0);
}

//function to take the separated request or complete request and parse it
int	Request::takingRequests(const String& receivedRequest)
{
	if (!this->requestLineDone)
	{
		if (parseRequestLine(receivedRequest))
			return 1;
	}
	if (!headersDone)
	{
		if (checkDuplicate(receivedRequest))
			return 1;
	}
	if (foundUri)
	{
		if (requestInProgress)
			requestVector = Utils::splitRequest(requestData, CRLF);
		else
		{
			separateRequest(receivedRequest);
			requestVector = Utils::splitRequest(headers, CRLF);
		}
		fillHeaders(requestVector); //fill the headers to the map
		requestIsWellFormed(); //check if the request is well formed
		receivecount++;
	}
	return 0;
}
