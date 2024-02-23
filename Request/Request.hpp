/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/23 00:54:59 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Config.hpp"
#include <fcntl.h>
#include <sys/stat.h> // For mkdir


/* 

GET / HTTP/1.1

POST / HTTP/1.1

host:localhost:5002
connection:keep-alive
accept-encoding:gzip, deflate, br
Content-Length: 11
Content-Type: text/plain

Hello world
*/

enum Errors
{
	NotImplemented = 501,
	BadRequest = 400,
	RequestURITooLong = 414,
	RequestEntityTooLarge = 413,
	NotFound = 404,
	MovedPermanently = 301,
	MethodNotAllowed = 405,
};

#define CR '\r'
#define CRLF "\r\n"
#define MAX_BODY_SIZE 100000000
#define ALLOWED_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJ\
							KLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%"

class Config;

class Request
{
	private:
		//status
		int _status;
		bool _bodyDone;
		bool _foundUri;
		int	_detectHost;
		bool _isComplete;
		int	_receivecount;
		bool _headersDone;
		bool _requestLineDone;
		bool requestInProgress;
		bool _requestIsWellFormed;
		// content length
		unsigned int	_contentLength;
		Config _config;
		//maps
		std::map<std::string, std::string> _headers;
		std::map<std::string, std::string> _requestLine;
		//vector
		std::vector<std::string>	_requestVector;
		//strings
		std::string _body;
		std::string	headers;
		std::string _params;
		std::string	_requestData;
		std::string _boundaryName;
	public:
	/* *************************** constructors ****************************** */
		Request();
		~Request();

	typedef std::invalid_argument InvalidRequest;

	/* *************************** methods ********************************* */
		void	findUri();
		void	boundary();
		void	parseBody();
		void	parseBoundary();
		void	ContentLength();
		void	parseChunkedBody();
		void	parseContentType();
		void	parseContentLength();
		void	requestIsWellFormed();
		void	parseTransferEncoding();
		void	separateRequest(std::string receivedRequest);
		void	fillHeaders(std::vector<std::string> headers);
		void	fillRequestLine(const std::string& requestLine);
		int		parseRequestLine(const std::string& requestLine);
		int		checkDuplicate(const std::string& receivedRequest);
		int		takingRequests(const std::string& receivedRequest);
		std::string	pripareFileName(std::string line, bool &initialFile);
		void	parseRequest(const std::string& receivedRequest, const Config& config);
	/* *************************** getters ************************************ */
		const int& getStatus() const;
		const std::string& getBody() const;
		const std::map<std::string, std::string>& getHeaders() const;
		const std::map<std::string, std::string>& getRequestLine() const;
		const bool& getRequestIsWellFormed() const;
		const bool& getBodyDone() const;
		const bool& getHeadersDone() const;
		const bool& getRequestLineDone() const;
		const bool& getFoundUri() const;
};
