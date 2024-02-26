/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:23:29 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/26 02:32:16 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to Check if transfer encoding is chunked Or not
void	Request::parseTransferEncoding()
{
	if (_headers["transfer-encoding"] != "chunked")
		_status = NotImplemented;
}

//function to check if the request Line is well formed And Set the status To true If all is well
void	Request::requestIsWellFormed()
{
	if (_requestLine["path"].size() > 2048)
		_status = RequestURITooLong;
	if (_requestLine["path"].find_first_not_of(ALLOWED_CHARACTERS) != std::string::npos)
		_status = BadRequest;
	if (_requestLine["method"] == "POST")
	{
		if (_headers.find("content-length") != _headers.end())
			parseContentLength();
		else if (_headers.find("content-type") != _headers.end()
				&& _headers.find("transfer-encoding") == _headers.end())
			parseContentType();
		else if (_headers.find("transfer-encoding") != _headers.end())
			parseTransferEncoding();
		else
			_status = BadRequest;
	}
	_requestIsWellFormed = true;
}

//function to find the uri in the config file and set the status to true if found
void	Request::findUri()
{
	std::string uri = _requestLine["path"];
	ServersVector ref = _config.getServers();
	ServersVector::iterator s_iter = ref.begin();

	for (; s_iter != ref.end(); s_iter++)
	{
		LocationsVector loc = s_iter->getLocations();
		LocationsVector::iterator l_iter = loc.begin();
		for (; l_iter != loc.end(); l_iter++)
		{
			if (uri == l_iter->getPrefix())
			{
				std::cout << "Matched: " << l_iter->getPrefix() << std::endl;
				_foundUri = true;
				return;
			}
		}
	}
	if (_foundUri == false)
		_status = NotFound;
}

//function to separate the request line And the headers from the request
void	Request::separateRequest(std::string receivedRequest)
{
	size_t pos = receivedRequest.find(CRLF CRLF);
	if (pos != std::string::npos)
	{
		this->headers = receivedRequest.substr(0, pos + 4);
		_body = receivedRequest.substr(pos + 4);
	}
	else
		_status = BadRequest;
}

//function to parse the Body of the request if method is POST
void	Request::parseBody()
{
	if (_requestLine["method"] == "POST"
			&& _headers.find("content-length") != _headers.end())
			ContentLength();
	else if (_requestLine["method"] == "POST"
			&& _headers.find("transfer-encoding") != _headers.end())
			parseChunkedBody();
	else if (_requestLine["method"] == "POST")
			parseBoundary();
}
//function to parse the request line and fill it to the map and return 1 if the request line is separated
int	Request::parseRequestLine(const std::string& requestLine)
{
	if (!requestLine.size())
		return 0;
	if (requestLine.find("\r\n\r\n") == std::string::npos)
	{
		_requestVector = Utils::splitRequest(requestLine, CRLF);
		fillRequestLine(_requestVector[0]); //fill the request line
		requestInProgress = true;
		return 1;
	}
	else
	{
		_requestVector = Utils::splitRequest(requestLine, CRLF);
		fillRequestLine(_requestVector[0]); //fill the request line
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
					_status = BadRequest;
		}
		if (receivedRequest.find("host") != std::string::npos)
			_detectHost++;
		return (_requestData += receivedRequest, _receivecount++, 1);
	}
	return (0);
}

//function to take the separated request or complete request and parse it
int	Request::takingRequests(const std::string& receivedRequest)
{
	if (!_requestLineDone)
	{
		if (parseRequestLine(receivedRequest))
			return 1;
	}
	if (!_headersDone)
	{
		if (checkDuplicate(receivedRequest))
			return 1;
	}
	if (_foundUri)
	{
		if (requestInProgress)
			_requestVector = Utils::splitRequest(_requestData, CRLF);
		else
		{
			separateRequest(receivedRequest);
			_requestVector = Utils::splitRequest(headers, CRLF);
		}
		fillHeaders(_requestVector); //fill the headers to the map
		requestIsWellFormed(); //check if the request is well formed
		_receivecount++;
	}
	return 0;
}
