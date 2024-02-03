/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/03 00:43:35 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/Utils.hpp"
#include "Request.hpp"

std::map<std::string, std::string> Request::_headers;
std::string Request::_body;
std::string Request::_method;
std::string Request::_path;
std::string Request::_version;
int Request::_status;

//group this methods inside pragma region
#pragma region Request

Request::Request()
{
	_headers.clear();
	_body.clear();
	_method.clear();
	_path.clear();
	_version.clear();
	_status = 0;
}

Request::~Request()
{
}

/* *************************** getters *************************** */

std::string	Request::getMethod()
{
	return (_method);
}

std::string	Request::getPath()
{
	return (_path);
}

std::string	Request::getVersion()
{
	return (_version);
}

std::string	Request::getBody()
{
	return (_body);
}

std::map<std::string, std::string>	Request::getHeaders()
{
	return (_headers);
}

int	Request::getStatus()
{
	return (_status);
}

#pragma endregion

/* *************************** methods *************************** */

void	Request::fillHeaders(std::vector<std::string> headers)
{
	if (headers.back() != "\r\n")
	{
		_status = BadRequest;
		throw std::exception();
	}
	std::vector<std::string>::iterator it;
	it = headers.begin();
	it++;
	for (; it != headers.end(); it++)
	{
		Utils::toLower(*it);
		std::string key;
		std::string value;
		size_t pos =  it->find(':');
		if (pos != std::string::npos)
		{
			key = it->substr(0, pos);
			value = Utils::strTrim(it->substr(pos + 2), '\r');
			_headers[key] = value;
		}
	}
}

void	Request::is_req_well_formed()
{
	std::map<std::string, std::string>::iterator it;

	it = _headers.find("transfer-encoding");
	if (it != _headers.end())
	{
		if (it->second != "chunked")
		{
			_status = NotImplemented;
			return;
		}
		std::cout << "chunked exists" << std::endl;
	}
	if (_method == "POST")
	{
		it = _headers.find("content-length");
		if (it == _headers.end())
		{
			it = _headers.find("transfer-encoding");
			if (it == _headers.end())
			{
				_status = BadRequest;
				return;
			}
		}
		else
		{
			//handle body here	
		}
		
	}
}

void	Request::parseRequest(std::string buffer)
{
	checkRequirements(buffer);
	Request::fillHeaders(Utils::splitRequest(buffer, "\r\n"));
	Request::is_req_well_formed();
}

void	Request::checkRequirements(std::string buffer)
{
	//methods = GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE

	std::vector<std::string> request = Utils::splitRequest(buffer, "\r\n");
	std::string method = request[0];
	std::vector<std::string> methodSplit = Utils::split(method, ' ');
	if (methodSplit.size() != 3)
	{
		_status = InvalidRequest;
		throw std::exception();
	}
	_method = methodSplit[0];
	_path = methodSplit[1];
	_version = methodSplit[2];
	
	if (_method != "GET" &&  _method != "POST" && _method != "DELETE" )
	{
		_status = MethodNotAllowed;
		throw std::exception();
	}
	if (_version != "HTTP/1.1")
	{
		_status = InvalidVersion;
		throw std::exception();
	}
	if (_path[0] != '/')
	{
		_status = InvalidPath;
		throw std::exception();
	}
}
