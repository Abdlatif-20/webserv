/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/10 14:44:59 by aben-nei         ###   ########.fr       */
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
	OK = 200,
	NotFound = 404,
	BadRequest = 400,
	LengthRequired = 411,
	MovedPermanently = 301,
	MethodNotAllowed = 405,
	NotImplemented = 501,
	RequestURITooLong = 414,
	RequestEntityTooLarge = 413,
};

#define CR '\r'
#define CRLF "\r\n"
#define MAX_BODY_SIZE 100000000
#define ALLOWED_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJ\
							KLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%"

typedef std::vector<std::string> Vector;
typedef std::map<std::string, std::string> Map;
typedef std::invalid_argument InvalidRequest;

class Config;

class Request
{
	private:
		//status
		int status;
		bool foundUri;
		bool bodyDone;
		int	detectHost;
		bool multipart;
		bool _setLength;
		bool isComplete;
		int	receivecount;
		bool headersDone;
		bool requestLineDone;
		bool requestInProgress;
		bool requestLineInProgress;
		bool _requestIsWellFormed;
		unsigned int remainingChunkLength;
		// content length
		unsigned int	contentLength;
		unsigned int	sizeBoundary;
		ServerContext serverCTX;
		LocationContext locationCtx;
		//maps
		Map _headers;
		Map requestLine;
		Map params;
		//vector
		Vector	requestVector;
		Vector	files;
		//strings
		std::string _path;
		std::string _body;
		std::string	headers;
		std::string	requestData;
		std::string	requestLineData;
		std::string boundaryName;
		std::string _chunkedName;
		/* *************************** methods ********************************* */
			void	findUri();
			void	parseUri();
			void	parseBody();
			void	fillParams();
			void	parseBoundary();
			void	ContentLength();
			void	parseChunkedBody();
			void	parseContentType();
			void	setUploadingPath();
			void	parseContentLength();
			void	requestIsWellFormed();
			void	parseTransferEncoding();
			void	isMethodAllowedInLocation();
			void	fillHeaders(Vector headers);
			bool directoryExists(const char *path);
			std::string	prepareFileName(std::string line);
			unsigned int convertToDecimal(std::string hex);
			std::string& isExist(Map& headers, std::string key);
			void	separateRequest(std::string receivedRequest);
			void	fillRequestLine(const std::string& requestLine);
			int		parseRequestLine(const std::string& requestLine);
			int		checkDuplicate(const std::string& receivedRequest);
			int		takingRequests(const std::string& receivedRequest);
			void	preparLengthAndName(size_t pos, std::string& length, std::ofstream& file);
			bool requestIscomplete;
	public:
	/* *************************** constructors ****************************** */
	
		Request();
		~Request();
		Request(const Request& obj);
		Request& operator=(const Request& obj);

	/* *************************** methods ****************************** */

		void	parseRequest(const std::string& receivedRequest, const ServerContext& serverCTX);
		void	resetRequest();

	/* *************************** getters ************************************ */
		const int& getStatus() const;
		void setStatus(int status);
		const std::string& getBody() const;
		const Map& getHeaders() const;
		const Map& getRequestLine() const;
		const bool& getRequestIsWellFormed() const;
		const bool& getBodyDone() const;
		const bool& getHeadersDone() const;
		const std::string& getHeaderByName(const std::string& name) const;
		const bool& getRequestLineDone() const;
		const bool& getFoundUri() const;

		bool	isDone(void) const;
		bool	hostIsDetected(void) const;
		const std::string& getMethod() const;
		const std::string& getHost() const;
		const LocationContext& getLocationCtx() const;
};
