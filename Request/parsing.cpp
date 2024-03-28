/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:23:29 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/28 00:43:14 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to set the path to the upload store
void	Request::setUploadingPath()
{
	try
	{
		this->_path = locationCTX.getUploadStore();
	}
	catch (std::exception& e)
	{
		status = InternalServerError;
		requestIscomplete = true;
	}
}

//function to Check if transfer encoding is chunked Or not
void	Request::parseTransferEncoding()
{
	if (_headers["transfer-encoding"] != "chunked")
		return (this->status = NotImplemented, requestIscomplete = true, void());
}

void	Request::isMethodAllowedInLocation()
{
	Vector allowedMethods = locationCTX.getAllowedMethods();
	if (std::find(allowedMethods.begin(), allowedMethods.end(), requestLine["method"]) == allowedMethods.end())
		return (this->status = MethodNotAllowed, requestIscomplete = true, void());
	_requestIsWellFormed = true;
}

//function to check if the request Line is well formed And Set the status To true If all is well
void	Request::requestIsWellFormed()
{
	if (this->status != OK)
		return;
	if (this->requestLine["path"].size() > 2048)
		return (this->status = RequestURITooLong, requestIscomplete = true, void());
	// if (this->requestLine["path"].find_first_not_of(ALLOWED_CHARACTERS) != String::npos)
	// 	return (this->status = BadRequest, requestIscomplete = true, void());
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
	isMethodAllowedInLocation();
	if (this->requestLine["method"] != "POST" && this->_requestIsWellFormed
			&& this->headersDone && this->requestLineDone)
		this->requestIscomplete = true;
}

//function to find the uri in the config file and set the status to true if found
void	Request::findUri()
{
	/* cast from context to SERVERCTX */
	std::string uri = requestLine["path"];
	LocationContext locationCtx = serverCTX.matchLocation(uri);
	if (locationCtx.getPrefix() != "")
	{
		this->locationCTX = locationCtx;
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

bool	uriEscapedRoot(const std::string& path)
{
	StringVector spl_vec = Utils::split(path, '/');
	StringVector::iterator it = spl_vec.begin();
	int a = 0;
	while (it != spl_vec.end())
	{
		if (*it != ".." && *it != ".")
			a++;
		else if (*it == "..")
			a--;
		if (a < 0)
			return true;
		it++;
	}
	return false;
}

void	Request::parseUri()
{
	if (requestLine["path"].find("%") != String::npos)
		requestLine["path"] = Utils::urlDecoding(requestLine["path"]);
	if (uriEscapedRoot(requestLine["path"]))
	{
		status = BadRequest;
		requestIscomplete = true;
	}
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
		&& _headers.find("content-type") != _headers.end() && _headers.find("transfer-encoding") != _headers.end())
			return (this->status = NotImplemented, requestIscomplete = true, void());
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
		if (this->requestVector.size() == 0)
			return (this->status = BadRequest, requestIscomplete = true, 0);
		if (this->requestVector.size() != 1)
		{
			for (size_t i = 1; i < this->requestVector.size(); i++)
				requestData += this->requestVector[i] + CRLF;
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
int	Request::takingRequests(String receivedRequest)
{
	if (!this->requestLineDone)
	{
		if (parseRequestLine(receivedRequest) && requestData.empty())
			return 1;
		if (!requestData.empty())
		{
			receivedRequest = requestData;
			requestData.clear();
		}
	}
	if (!headersDone)
	{
		if (checkDuplicate(receivedRequest))
			return 1;
	}
	if (requestInProgress)
		requestVector = Utils::splitRequest(requestData, CRLF);
	else
	{
		separateRequest(receivedRequest);
		requestVector = Utils::splitRequest(headers, CRLF);
	}
	fillHeaders(requestVector); //fill the headers to the map
	findUri();
	parseUri();
	requestIsWellFormed(); //check if the request is well formed
	receivecount++;
	return 0;
}
