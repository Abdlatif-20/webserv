/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/12 13:05:52 by aben-nei         ###   ########.fr       */
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


enum Errors
{
	OK = 200,
	NotFound = 404,
	BadRequest = 400,
	NotImplemented = 501,
	LengthRequired = 411,
	MovedPermanently = 301,
	MethodNotAllowed = 405,
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
typedef std::string String;
typedef std::invalid_argument InvalidRequest;

class Config;

class Request
{
	private:
		//status
		int		status;
		bool	foundUri;
		bool	bodyDone;
		bool	multipart;
		int		detectHost;
		bool	_setLength;
		bool	headersDone;
		bool	isComplete;
		int		receivecount;
		bool	requestLineDone;
		bool	requestInProgress;
		bool	requestIscomplete;
		bool	requestLineInProgress;
		bool	_requestIsWellFormed;
		//unsigned int
		unsigned int	sizeBoundary;
		unsigned int	contentLength;
		unsigned int	remainingChunkLength;
		//config
		ServerContext serverCTX;
		LocationContext locationCtx;
		//maps
		Map	_headers;
		Map	requestLine;
		Map	params;
		//vector
		Vector	requestVector;
		Vector	files;
		//strings
		String	_path;
		String	_body;
		String	headers;
		String	requestData;
		String	requestLineData;
		String	boundaryName;
		String	_chunkedName;

		/* *************************** methods ********************************* */
			void			findUri();
			void			parseUri();
			void			parseBody();
			void			fillParams();
			void			parseBoundary();
			void			ContentLength();
			void			parseChunkedBody();
			void			parseContentType();
			void			setUploadingPath();
			void			parseContentLength();
			void			requestIsWellFormed();
			void			parseTransferEncoding();
			void			isMethodAllowedInLocation();
			void			fillHeaders(Vector headers);
			String			prepareFileName(String line);
			unsigned int	convertToDecimal(String hex);
			bool			directoryExists(const char *path);
			String& 		isExist(Map& headers, String key);
			void			separateRequest(String receivedRequest);
			void			fillRequestLine(const String& requestLine);
			int				parseRequestLine(const String& requestLine);
			int				checkDuplicate(const String& receivedRequest);
			int				takingRequests(const String& receivedRequest);
			void			preparLengthAndName(size_t pos, String& length, std::ofstream& file);
	public:
	/* *************************** constructors ****************************** */
	
		Request();
		~Request();
		Request(const Request& obj);
		Request& operator=(const Request& obj);

	/* *************************** methods ****************************** */

		void	parseRequest(const String& receivedRequest, const ServerContext& serverCTX);
		void	resetRequest();

	/* *************************** getters ************************************ */
		bool					isDone(void) const;
		const String 			getHost(void) const;
		const String&			getBody(void) const;
		void					setStatus(int status);
		const String 			getMethod(void) const;
		const int&				getStatus(void) const;
		const Map& 				getHeaders(void) const;
		const bool& 			getBodyDone(void) const;
		const bool& 			getFoundUri(void) const;
		const bool& 			getHeadersDone(void) const;
		const Map& 				getRequestLine(void) const;
		const LocationContext&	getLocationCtx(void) const;
		bool					hostIsDetected(void) const;
		const bool& 			getRequestLineDone(void) const;
		const bool& 			getRequestIsWellFormed(void) const;
		const String 			getHeaderByName(const String& name) const;

};
