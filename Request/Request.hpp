/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/11 19:40:29 by aben-nei         ###   ########.fr       */
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
		bool _bodyDone;
		bool _headersDone;
		bool _requestLineDone;
		bool _requestIsWellFormed;
		bool _foundUri;
		int	_receivecount;
		int _status;
		// content length
		unsigned int	_contentLength;
		//maps
		std::map<std::string, std::string> _headers;
		std::map<std::string, std::string> _requestLine;
		//strings
		std::string _body;
		std::string _params;
		std::string	headers;
		std::string _boundaryName;
		std::string	_configPath;
	public:
	/* *************************** constructors *************************** */
		Request();
		~Request();

	typedef std::invalid_argument InvalidRequest;
	/* *************************** methods *************************** */
		void	separateRequest(std::string receivedRequest);
		void	parseRequest(const std::string& receivedRequest, char *configPath);
		void	fillHeaders(std::vector<std::string> headers);
		void	requestIsWellFormed();
		void	fillRequestLine(const std::string& requestLine);
		void	parseContentLength();
		void	parseContentType();
		void	parseTransferEncoding();
		void	parseBody();
		// function to parse boundary
		void	ContentLength();
		void	parseBoundary();
		void	boundary();
		//Uri
		void	findUri();
		void	matchUriRequest();
		

	/* *************************** getters *************************** */
		std::string getBody();
		std::map<std::string, std::string> getRequestLine();
		std::map<std::string, std::string> getHeaders();
		int getStatus();

};
