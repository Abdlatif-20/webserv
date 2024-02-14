/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/14 04:12:29 by aben-nei         ###   ########.fr       */
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


/*

GET / HTTP/1.1
host:localhost:5002
connection:keep-alive

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
#define MAX_BODY_SIZE 1000000
#define ALLOWED_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJ\
							KLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%"

class Request
{
	private:
		//status
		int _status;
		bool _bodyDone;
		bool _foundUri;
		int	_detectHost;
		int	_receivecount;
		bool _headersDone;
		bool _requestLineDone;
		int	_detectRequestLine;
		bool _receivedProgress;
		bool _requestIsWellFormed;
		// content length
		unsigned int	_contentLength;
		//maps
		std::map<std::string, std::string> _headers;
		std::map<std::string, std::string> _requestLine;
		//vector
		std::vector<std::string>	_requestVector;
		//strings
		std::string _body;
		std::string	headers;
		std::string _params;
		std::string	_configPath;
		std::string	_requestData;
		std::string _boundaryName;
	public:
	/* *************************** constructors *************************** */
		Request();
		~Request();

	typedef std::invalid_argument InvalidRequest;

	/* *************************** methods *************************** */
		void	findUri();
		void	boundary();
		void	parseBody();
		void	parseBoundary();
		void	ContentLength();
		void	matchUriRequest();
		void	parseContentType();
		void	parseContentLength();
		void	requestIsWellFormed();
		void	parseTransferEncoding();
		void	separateRequest(std::string receivedRequest);
		void	fillHeaders(std::vector<std::string> headers);
		void	fillRequestLine(const std::string& requestLine);
		int		parseRequestLine(const std::string& requestLine);
		void	parseRequest(const std::string& receivedRequest, char *configPath);

	/* *************************** getters *************************** */
		int getStatus();
		std::string getBody();
		std::map<std::string, std::string> getHeaders();
		std::map<std::string, std::string> getRequestLine();

};
