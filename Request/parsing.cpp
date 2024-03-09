/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:23:29 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/09 16:13:46 by aben-nei         ###   ########.fr       */
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
	{
		this->status = NotImplemented;
		requestIscomplete = true;
	}
}

//function to check if the request Line is well formed And Set the status To true If all is well
void	Request::requestIsWellFormed()
{
	if (this->requestLine["path"].size() > 2048)
		return (this->status = RequestURITooLong, requestIscomplete = true, void());
	if (this->requestLine["path"].find_first_not_of(ALLOWED_CHARACTERS) != std::string::npos)
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
}

//function to find the uri in the config file and set the status to true if found
void	Request::findUri()
{
	std::string uri = requestLine["path"];
	LocationContext _locationCtx = serverCTX.matchLocation(uri);
	if (_locationCtx.getPrefix() != "")
	{
		this->locationCtx = _locationCtx;
		foundUri = true;
		return;
	}
	status = NotFound;
	requestIscomplete = true;
}

//function to separate the request line And the headers from the request
void	Request::separateRequest(std::string receivedRequest)
{
	size_t pos = receivedRequest.find("\r\n\r\n");
	if (pos != std::string::npos)
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
	if (this->requestLine["path"].find("%") != std::string::npos)
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
int	Request::parseRequestLine(const std::string& requestLine)
{
	if (!requestLine.size())
		return 0;
	if (requestLine.find("\r\n\r\n") == std::string::npos)
	{
		if (requestLine.find("\r\n") == std::string::npos)
		{
			requestLineInProgress = true;
			requestLineData += requestLine;
			return 1;
		}
		if (requestLineInProgress)
		{
			requestLineData += requestLine;
			this->requestVector = Utils::splitRequest(requestLineData, CRLF);
			fillRequestLine(this->requestVector[0]); //fill the request line
			this->requestInProgress = true;
			requestLineInProgress = false;
		}
		else
		{
			this->requestVector = Utils::splitRequest(requestLine, CRLF);
			fillRequestLine(this->requestVector[0]); //fill the request line
			this->requestInProgress = true;
		}
		return 1;
	}
	else
	{
		this->requestVector = Utils::splitRequest(requestLine, CRLF);
		fillRequestLine(this->requestVector[0]); //fill the request line
	}
	return 0;
}

//check if the request line or host is duplicated
int	Request::checkDuplicate(const std::string& receivedRequest)
{
	if (receivedRequest != CRLF && requestInProgress)
	{
		size_t pos = receivedRequest.find(" ");
		if (pos != std::string::npos)
		{
			std::string value = receivedRequest.substr(0, pos);
			if (value.find("GET") != std::string::npos
				|| value.find("POST") != std::string::npos
				|| value.find("DELETE") != std::string::npos
				|| value.find("HEAD") != std::string::npos
				|| value.find("PUT") != std::string::npos
				|| value.find("CONNECT") != std::string::npos
				|| value.find("OPTIONS") != std::string::npos
				|| value.find("TRACE") != std::string::npos)
					return (this->status = BadRequest, requestIscomplete = true, 0);
		}
		if (receivedRequest.find("host") != std::string::npos)
			detectHost++;
		return (requestData += receivedRequest, receivecount++, 1);
	}
	return (0);
}

//function to take the separated request or complete request and parse it
int	Request::takingRequests(const std::string& receivedRequest)
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
	// if (foundUri)
	// {
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
	// }
	return 0;
}
