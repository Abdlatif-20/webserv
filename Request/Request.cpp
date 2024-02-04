/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/04 13:34:55 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utils/Utils.hpp"
#include "Request.hpp"

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

/* *************************** exceptions *************************** */

#pragma region RequestExceptions
const char* Request::InvalidPathException::what() const throw()
{
	return "Invalid Path";
}

const char* Request::InvalidMethodException::what() const throw()
{
	return "Invalid Method";
}

const char* Request::InvalidVersionException::what() const throw()
{
	return "Invalid Version";
}

const char* Request::InvalidHeaderException::what() const throw()
{
	return "Invalid Header";
}

const char* Request::InvalidBodyException::what() const throw()
{
	return "Invalid Body";
}

const char* Request::InvalidRequestException::what() const throw()
{
	return "Invalid Request";
}

#pragma endregion

/* *************************** methods *************************** */

void	Request::fillHeaders(std::vector<std::string> headers)
{
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

// void	Request::fillBuddy(std::string req)
// {
// 	size_t pos = req.find("Content-Length");
// 	// if (pos != std::string::npos)
// 	// {
// 	// 	_body = req.substr(pos + 16);
// 	// }
// 	// std::cout << "body: " << _body << std::endl;
// }

void	Request::handelTransferEncoding()
{
	// std::ofstream file;

	// file.open("file");
	// if (!file)
	// {
		
	// }
}

void	Request::is_req_well_formed()
{
	std::map<std::string, std::string>::iterator it;
	bool isTransferEncoding = true;
	bool isContentLength = true;
	bool isContentType = true;

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
			isContentLength = false;
			it = _headers.find("transfer-encoding");
			if (it == _headers.end())
			{
				isTransferEncoding = false;
				it = _headers.find("content-type");
				if (it == _headers.end())
				{
					isContentType = false;
					_status = BadRequest;
					return;
				}
			}
		}
		// if (isContentLength == true)
		// 	handelContentLength();
		// else if (isTransferEncoding == true)
		// 	handelTransferEncoding();
		// else if (isContentType == true)
		// 	handelContentType();
		
	}
}

void	Request::parseRequest(std::string buffer)
{
	checkRequirements(buffer);
	Request::fillHeaders(Utils::splitRequest(buffer, "\r\n"));
	Request::is_req_well_formed();
	// std::cout<<"-------- map -------------";
	// Utils::printMap(_headers);
	// std::cout<<"----------------------------";
	// Request::fillBuddy(buffer);
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
		throw Request::InvalidRequestException();
	}
	_method = methodSplit[0];
	_path = methodSplit[1];
	_version = methodSplit[2];

	if (_method != "GET" &&  _method != "POST" && _method != "DELETE" )
	{
		_status = MethodNotAllowed;
		throw Request::InvalidMethodException();
	}
	if (_version != "HTTP/1.1")
	{
		_status = InvalidVersion;
		throw Request::InvalidVersionException();
	}
	if (_path[0] != '/')
	{
		_status = InvalidPath;
		throw Request::InvalidPathException();
	}
}
