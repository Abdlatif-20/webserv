/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/28 05:57:20 by aben-nei         ###   ########.fr       */
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
#include "Server.hpp"
#include <fcntl.h>
#include <sys/stat.h> // For mkdir


#define BUFFER_SIZE 102400

#define CR '\r'
#define CRLF "\r\n"
#define ALLOWED_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJ\
							KLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=% "

typedef std::vector<std::string> Vector;
typedef std::map<std::string, std::string> Map;
typedef std::string String;

class Core;
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
		bool	_requestIsWellFormed;
		bool	_chunkedComplete;
		bool	_BoundaryComplete;
		bool	requestLineInProgress;

		char	buffer[BUFFER_SIZE];
		int		tmpFile;
		// serverctxs range
		size_t	serv_end;
		size_t	serv_begin;
		//unsigned int
		unsigned int	sizeBoundary;
		unsigned int	contentLength;
		size_t			remainingChunkLength;
		unsigned int	remainingChunk;
		//config
		ServerContext serverCTX;
		LocationContext locationCTX;
		// Context			*context;
		// LocationContext	locationCtx;
		//maps
		Map	_headers;
		Map	requestLine;
		//vector
		Vector	requestVector;
		//strings
		String	_path;
		String	_pathTmpFile;
		String	_params;
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
			int				preparName();
			void			parseBoundary();
			void			ContentLength();
			void			parseChunkedBody();
			void			parseContentType();
			void			setUploadingPath();
			void			parseContentLength();
			void			requestIsWellFormed();
			void			createChunkedTmpFile();
			void			createBoundaryTmpFile();
			void			parseTransferEncoding();
			bool			writeInfile(int fdFile);
			void			isMethodAllowedInLocation();
			void			fillHeaders(Vector headers);
			String			prepareFileName(String line);
			unsigned int	convertToDecimal(String hex);
			String& 		isExist(Map& headers, String key);
			bool			fileExists(const std::string& fileName);
			void			separateRequest(String receivedRequest);
			void			fillRequestLine(const String& requestLine);
			int				parseRequestLine(const String& requestLine);
			int				checkDuplicate(const String& receivedRequest);
			int				takingRequests(String receivedRequest);
			void			preparLength(String& length);
			void			readBytes(int fd, ssize_t& bytesRead);
	public:
	/* *************************** constructors ****************************** */
	
		Request();
		~Request();
		Request(const Request& obj);
		Request& operator=(const Request& obj);

		
		static std::vector<Server>	servers;

	/* *************************** methods ****************************** */
		void	resetRequest();
		void	selectServerContext(const String& host);
		void	setServerCTXEnd(size_t i);
		void	setServerCTXBegin(size_t i);
		void	parseRequest(const std::string& receivedRequest, ServerContext serverCTX);

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
		const std::string		getRequestPath(void) const;
		const ServerContext&	getServerCTX(void) const;
		const LocationContext&	getLocationCtx(void) const;
		bool					hostIsDetected(void) const;
		const bool& 			getRequestLineDone(void) const;
		const bool& 			getRequestIsWellFormed(void) const;
		const String 			getHeaderByName(const String& name) const;

};