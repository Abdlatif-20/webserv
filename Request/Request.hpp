/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/09 13:32:40 by aben-nei         ###   ########.fr       */
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

enum Errors
{
	NotImplemented = 501,
	BadRequest = 400,
	RequestURITooLong = 414,
	RequestEntityTooLarge = 413,
	NotFound = 404,
	MovedPermanently = 301,
	MethodNotAllowed = 405,
	//
	InvalidPath = 1,
	InvalidMethod = 2,
	InvalidVersion = 3,
	InvalidHeader = 4,
	InvalidBody = 5,
	InvalidRequest = 6,
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
		int	_receivecount;
		int _status;
		std::map<std::string, std::string> _requestLine;
		std::string	headers;
		std::string _requestData;
		std::string _body;
		unsigned int	_contentLength;
		std::map<std::string, std::string> _headers;
		std::string _params;
	public:
	/* *************************** constructors *************************** */
		Request();
		~Request();

	typedef std::invalid_argument InvalidRequest;
	/* *************************** methods *************************** */
		void	separateRequest(std::string receivedRequest);
		void	parseRequest(const std::string& receivedRequest);
		void	fillHeaders(std::vector<std::string> headers);
		void	requestIsWellFormed();
		void	fillRequestLine(const std::string& requestLine);
		void	parseContentLength();
		void	parseContentType();
		void	parseTransferEncoding();
		void	matchUriRequest();
		void	parseBody();
		// function to parse boundary
		void	parseBoundary();

	/* *************************** getters *************************** */
		std::string getBody();
		std::map<std::string, std::string> getRequestLine();
		std::map<std::string, std::string> getHeaders();
		int getStatus();

};
