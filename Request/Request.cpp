/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:56:37 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/09 22:01:42 by aben-nei         ###   ########.fr       */
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
			value = Utils::strTrim(it->substr(pos + 2), CR);
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
		throw InvalidRequest("Invalid body(Invalid Content-Length)");
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

void	Request::parseRequest(const std::string& receivedRequest)
{
	std::vector<std::string> requestLineVector;
	std::srand(time(0));
	if (!_requestLineDone && !_headersDone && !_requestIsWellFormed)
	{
		separateRequest(receivedRequest); //separate the request line from the headers
		requestLineVector = Utils::splitRequest(headers, CRLF);
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
	// Utils::printFile("boundary.txt");
}

void	Request::boundary()
{

}

void	Request::parseBoundary()
{
	int random = std::rand();
	std::string randomStr = Utils::intToString(random);
	std::ofstream file("boundary" + randomStr + ".txt", std::ios::app);
	bool isComplete = false;
	std::string StartBoundary = "--" + _headers["content-type"].substr(30);
	std::string EndBoundary = StartBoundary + "--";
	if (!file.is_open())
	{
		_status = BadRequest;
		throw InvalidRequest("can't open file");
	}
	if (_body.find(EndBoundary) == std::string::npos)
	{
		file << _body;
		_receivecount++;
	}
	else
	{
		file << _body.substr(0, _body.find(EndBoundary) + EndBoundary.size() + 2);
		_body = _body.substr(_body.find(EndBoundary) + EndBoundary.size() + 2);
		isComplete = true;
	}
	if (isComplete)
	{
		file.close();
		boundary();
		_bodyDone = true;
		std::cout << "Body parsed" << std::endl;
	}
}

/*

POST / HTTP/1.1
x-access-token: goldapi-jb5rljwaivri-io
User-Agent: PostmanRuntime/7.33.0

Postman-Token: 6cc92122-4a1d-48f6-ad15-3ba1638c8ae9
Host: localhost:5008
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Content-Type: multipart/form-data; boundary=--------------------------512122774062530383246207

----------------------------512122774062530383246207
Content-Disposition: form-data; name="test"

gfd
----------------------------512122774062530383246207
Content-Disposition: form-data; name="test1"

455564sddsf
----------------------------512122774062530383246207
Content-Disposition: form-data; name="test2"

sds5fds5f
----------------------------512122774062530383246207
Content-Disposition: form-data; name="test3"

5dcdsfds5
----------------------------512122774062530383246207--

*/

void	Request::ContentLength()
{
	std::string randomStr = Utils::intToString(std::rand() % 1000);
	std::ofstream file("body" + randomStr + ".txt", std::ios::app);
	if (!file.is_open())
	{
		_status = BadRequest;
		throw InvalidRequest("can't open file");
	}
	if (_bodyDone == false)
	{
		if (_contentLength > _body.size())
		{
			file << _body;
			_contentLength -= _body.size();
			_receivecount++;
		}
		else if (_contentLength < _body.size())
		{
			file << _body.substr(0, _contentLength);
			_body = _body.substr(_contentLength);
			_contentLength = 0;
		}
	}
	if (_contentLength == 0)
	{
		file.close();
		_bodyDone = true;
		std::cout << "Body parsed" << std::endl;
	}
}

void	Request::parseBody()
{
	if (_requestLine["method"] == "POST"
			&& _headers.find("content-length") != _headers.end())
			ContentLength();
	else if (_requestLine["method"] == "POST"
			&& _headers.find("content-type") != _headers.end())
			parseBoundary();
}
