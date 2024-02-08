/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/08 22:48:48 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Request.hpp"

//group this methods inside pragma region
#pragma region Request

Request::Request()
{
	_status = 200;
	_bodyDone = false;
	_receivecount = 0;
	_contentLength = 0;
	_headersDone = false;
	_requestLineDone = false;
	_requestIsWellFormed = false;
}

Request::~Request()
{
}

/* *************************** getters *************************** */

std::string	Request::getBody()
{
	return (_body);
}

std::map<std::string, std::string>	Request::getHeaders()
{
	return (_headers);
}

std::map<std::string, std::string>	Request::getRequestLine()
{
	return (_requestLine);
}

int	Request::getStatus()
{
	return (_status);
}

#pragma endregion

/* *************************** methods *************************** */

//function to fill the headers to the map
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
	_headersDone = true;
}

//function to parse the content length
void	Request::parseContentLength()
{
	if (_headers["content-length"].find_first_not_of("0123456789") != std::string::npos)
	{
		_status = BadRequest;
		throw InvalidRequest("Invalid body");
	}
	if (std::stoi(_headers["content-length"]) > MAX_BODY_SIZE)
	{
		_status = RequestEntityTooLarge;
		throw InvalidRequest("Request Entity Too Large");
	}
	_contentLength = std::stoi(_headers["content-length"]);
}

//function to parse the content type
void	Request::parseContentType()
{
	if (_headers["content-type"].find("boundary") == std::string::npos)
	{
		_status = BadRequest;
		throw InvalidRequest("Invalid body(Invalid Content-Type)");
	}
}

//function to parse the transfer encoding
void	Request::parseTransferEncoding()
{
	if (_headers["transfer-encoding"] != "chunked")
	{
		_status = NotImplemented;
		throw InvalidRequest("Not Implemented(Transfer-Encoding not supported)");
	}
}

//function to check if the request is well formed
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

void	Request::fillRequestLine(const std::string& requestLine)
{
	std::vector<std::string> requestLineVector;
	std::map<std::string, std::string> requestLineMap;
	requestLineVector = Utils::split(requestLine, ' ');
	if (requestLineVector.size() != 3)
		 throw InvalidRequest("Invalid request line");
	if (requestLineVector[0] != "GET" && requestLineVector[0] != "POST"
		&& requestLineVector[0] != "DELETE")
	{
		if (requestLineVector[0] != "HEAD" && requestLineVector[0] != "PUT"
			&& requestLineVector[0] != "CONNECT" && requestLineVector[0] != "OPTIONS"
				&& requestLineVector[0] != "TRACE")
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
}

void	Request::matchUriRequest()
{
	std::string uri = "index.html";
	std::string path = _requestLine["path"];
	if (path == "/")
		_requestLine["path"] = uri;
	else
		_requestLine["path"] = path.substr(1);
	if (uri != _requestLine["path"])
	{
		_status = NotFound;
		throw InvalidRequest("404 Not Found");
	}
	std::cout << "Uri request matched: " << _requestLine["path"] << std::endl;
}

void	Request::separateRequest(std::string receivedRequest)
{
	size_t pos = receivedRequest.find("\r\n\r\n");
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

void	Request::parseRequest(const std::string& receivedRequest)
{
	std::vector<std::string> requestLineVector;
	if (!_requestLineDone && !_headersDone && !_requestIsWellFormed)
	{
		separateRequest(receivedRequest); //separate the request line from the headers
		requestLineVector = Utils::splitRequest(headers, "\r\n");
		fillRequestLine(requestLineVector[0]); //fill the request line
		fillHeaders(requestLineVector); //fill the headers
		requestIsWellFormed(); //check if the request is well formed
		_requestData = receivedRequest;
		_receivecount++;
	}
	// matchUriRequest();
	if (!_bodyDone)
	{
		if (_receivecount > 1)
			_body = receivedRequest;
		parseBody();
	}
}

void	Request::parseBoundary()
{
	
}

void	Request::parseBody()
{
	std::ofstream file;
	static std::string	body;
	if (_requestLine["method"] == "POST"
			&& _headers.find("content-length") != _headers.end())
	{
		file.open("c_l_body.txt");
		if (!file.is_open())
		{
			_status = BadRequest;
			throw InvalidRequest("can't open file");
		}
		if (_bodyDone == false)
		{
			body += _body;
			_receivecount++;
			if (body.size() == _contentLength)
			{
				file << body;
				file.close();
				_bodyDone = true;
				std::cout << "Body parsed" << std::endl;
			}
			return ;
		}
	}
	// else if (_requestLine["method"] == "POST"
	// 		&& _headers.find("content-type") != _headers.end())
	// 	parseBoundary();
}

/*

POST / HTTP/1.1
x-access-token: goldapi-jb5rljwaivri-io
User-Agent: PostmanRuntime/7.33.0

Postman-Token: 6cc92122-4a1d-48f6-ad15-3ba1638c8ae9
Host: localhost:5008
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Content-Type: multipart/form-data; boundary=--------------------------136639999040286537770224

----------------------------136639999040286537770224
Content-Disposition: form-data; name="dsdf"

dfg
----------------------------136639999040286537770224
Content-Disposition: form-data; name="fg"

fggh
----------------------------136639999040286537770224
Content-Disposition: form-data; name="jk"

hhh
----------------------------136639999040286537770224
Content-Disposition: form-data; name="kyhj"

hjhk
----------------------------136639999040286537770224--

*/