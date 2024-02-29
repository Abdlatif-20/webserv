/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:26:57 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/29 02:03:51 by aben-nei         ###   ########.fr       */
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
			value = Utils::strTrim(Utils::strTrim(it->substr(pos + 1), CR), ' ');
			_headers[key] = value;
		}
	}
	if (checkHostIsFound != 1)
		_status = BadRequest;
	_headersDone = true;
}

//function to Check if the request Line is well formed and fill it to the map
void	Request::fillRequestLine(const std::string& requestLine)
{
	if (!requestLine.size())
		return(_status = BadRequest, void());
	std::vector<std::string> requestLineVector;
	std::map<std::string, std::string> requestLineMap;
	requestLineVector = Utils::split(requestLine, ' ');
	if (requestLineVector.size() != 3)
		return(_status = BadRequest, void());
	if (requestLineVector.front() != "GET" && requestLineVector.front() != "POST"
		&& requestLineVector.front() != "DELETE")
	{
		if (requestLineVector.front() != "HEAD" && requestLineVector.front() != "PUT"
			&& requestLineVector.front() != "CONNECT" && requestLineVector.front() != "OPTIONS"
				&& requestLineVector.front() != "TRACE")
				_status = MethodNotAllowed;
		else
			_status = NotImplemented;
		return;
	}
	if (requestLineVector[1].front() != '/')
		return (_status = BadRequest, void());
	if (requestLineVector[2] != "HTTP/1.1")
		return (_status = BadRequest, void());
	requestLineMap["method"] = requestLineVector[0];
	requestLineMap["path"] = requestLineVector[1];
	_requestLineDone = true;
	_requestLine = requestLineMap;
	fillParams();
	findUri();
	parseUri();
}

void	Request::fillParams()
{
	std::vector<std::string> sepPath;
	std::string path = _requestLine["path"];
	sepPath = Utils::split(path, '?');
	_requestLine["path"] = sepPath[0];
	if (sepPath.size() > 1)
	{
		std::vector<std::string> sepParams;
		sepParams = Utils::split(sepPath[1], '&');
		for (size_t i = 0; i < sepParams.size(); i++)
		{
			std::vector<std::string> sepParam;
			sepParam = Utils::split(sepParams[i], '=');
			if (sepParam.size() == 2)
				_params[sepParam[0]] = sepParam[1];
		}
	}
}