/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:23:29 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/11 23:39:35 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to Check if transfer encoding is chunked Or not
void	Request::parseTransferEncoding()
{
	if (_headers["transfer-encoding"] != "chunked")
	{
		_status = NotImplemented;
		throw InvalidRequest("Not Implemented(Transfer-Encoding not supported)");
	}
}

//function to check if the request Line is well formed And Set the status To true If all is well
void	Request::requestIsWellFormed()
{
	if (_requestLine["path"].size() > 2048)
	{
		_status = RequestURITooLong;
		throw InvalidRequest("Request-URI Too Long");
	}
	if (_requestLine["path"].find_first_not_of(ALLOWED_CHARACTERS) != std::string::npos)
	{
		_status = BadRequest;
		throw InvalidRequest("Bad Request(Invalid path)");
	}
	if (_requestLine["method"] == "POST")
	{
		if (_headers.find("content-length") != _headers.end())
			parseContentLength();
		else if (_headers.find("content-type") != _headers.end())
			parseContentType();
		else if (_headers.find("transfer-encoding") != _headers.end())
			parseTransferEncoding();
		else
		{
			_status = BadRequest;
			throw InvalidRequest("Bad Request(Invalid body)");
		}
	}
	_requestIsWellFormed = true;
}

//function to find the uri in the config file and set the status to true if found
void	Request::findUri()
{
	std::string uri = _requestLine["path"];
	Config _config(_configPath);
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
	{
		_status = NotFound;
		throw InvalidRequest("URI not found");
	}
}

void	Request::matchUriRequest()
{
	Config _config(_configPath);
	ServersVector ref = _config.getServers();
	ServersVector::iterator s_iter = ref.begin();
	LocationsVector loc = s_iter->getLocations();
	LocationsVector::iterator l_iter = loc.begin();
	std::string uri = _requestLine["path"];
	std::string prefix = l_iter->getPrefix();

	if (uri.find(prefix) != std::string::npos)
	{
		std::cout << "Matched" << std::endl;
	}
	else
	{
		std::cout << "Not Matched" << std::endl;
	}
}

//function to separate the request line And the headers from the request
void	Request::separateRequest(std::string receivedRequest)
{
	size_t pos = receivedRequest.find(CRLF CRLF);
	if (pos != std::string::npos)
	{
		headers = receivedRequest.substr(0, pos + 4);
		_body = receivedRequest.substr(pos + 4);
	}
	else
	{
		_status = BadRequest;
		throw InvalidRequest("Invalid request");
	}

}

//function to parse the Body of the request
void	Request::parseBody()
{
	if (_requestLine["method"] == "POST"
			&& _headers.find("content-length") != _headers.end())
			ContentLength();
	else if (_requestLine["method"] == "POST"
			&& _headers.find("content-type") != _headers.end())
			parseBoundary();
}

//main function to parse the request
void	Request::parseRequest(const std::string& receivedRequest, char *configPath)
{
	// std::cout <<"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-"<< std::endl;
	// std::cout << "Request:\n" << receivedRequest << std::endl;
	// std::cout <<"-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-"<< std::endl;
	// exit(0);
	_configPath = configPath;
	std::srand(time(0));
	if (!_requestLineDone && !_headersDone && !_requestIsWellFormed)
	{
		std::vector<std::string> requestVector;
		separateRequest(receivedRequest); //separate the request line from the headers
		requestVector = Utils::splitRequest(headers, CRLF);
		fillRequestLine(requestVector[0]); //fill the request line
		fillHeaders(requestVector); //fill the headers
		requestIsWellFormed(); //check if the request is well formed
		_receivecount++;
	}
	if (!_bodyDone)
	{
		if (_receivecount > 1)
			_body = receivedRequest;
		parseBody();
	}
	// matchUriRequest();
	// Utils::printFile("boundary.txt");
}
