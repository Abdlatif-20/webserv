/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:26:57 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/15 07:45:52 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to fill the headers to the map
void	Request::fillHeaders(std::vector<std::string> headers)
{
	int checkHostIsFound = 0;
	std::vector<std::string>::iterator it;
	it = headers.begin();
	requestInProgress == false ? it++ : it;
	for (; it != headers.end(); it++)
	{
		Utils::toLower(*it);
		std::string key;
		std::string value;
		size_t pos =  it->find(':');
		if (pos != std::string::npos)
		{
			key = it->substr(0, pos);
			if (key == "host")
				checkHostIsFound++;
			value = Utils::strTrim(it->substr(pos + 1), CR);
			value = Utils::strTrim(value, ' ');
			_headers[key] = value;
		}
	}
	if (checkHostIsFound != 1)
	{
		_status = BadRequest;
		throw InvalidRequest("Host not found");
	}
	_headersDone = true;
}

//function to Check if the request Line is well formed and fill it to the map
void	Request::fillRequestLine(const std::string& requestLine)
{
	std::vector<std::string> requestLineVector;
	std::map<std::string, std::string> requestLineMap;
	requestLineVector = Utils::split(requestLine, ' ');
	if (requestLineVector.size() != 3)
	{
		_status = BadRequest;
		throw InvalidRequest("Invalid request line");
	}
	if (requestLineVector.front() != "GET" && requestLineVector.front() != "POST"
		&& requestLineVector.front() != "DELETE")
	{
		if (requestLineVector.front() != "HEAD" && requestLineVector.front() != "PUT"
			&& requestLineVector.front() != "CONNECT" && requestLineVector.front() != "OPTIONS"
				&& requestLineVector.front() != "TRACE")
			{
				_status = BadRequest;
				throw InvalidRequest("Invalid method");
			}
		throw InvalidRequest("Method not implemented");
	}
	if (requestLineVector[1].front() != '/')
	{
		_status = BadRequest;
		throw InvalidRequest("Invalid path");
	}
	if (requestLineVector[2] != "HTTP/1.1")
	{
		_status = BadRequest;
		throw InvalidRequest("Invalid version");
	}
	requestLineMap["method"] = requestLineVector[0];
	requestLineMap["path"] = requestLineVector[1];
	_requestLineDone = true;
	_requestLine = requestLineMap;
	findUri();
}
