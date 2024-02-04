/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/04 22:18:30 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <poll.h>
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

#define CRLF "\r\n"
#define MAX_BODY_SIZE 1000000
#define ALLOWED_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%"

class Request
{
	private:
		int _status;
		std::map<std::string, std::string> _requestLine;
		std::string _requestData;
		std::string _body;
		std::map<std::string, std::string> _headers;
		std::string _params;
	public:
		Request();
		~Request();

	typedef std::invalid_argument InvalidRequest;
	/* *************************** methods *************************** */
		void	parseRequest(const std::string& receivedRequest);
		void	fillHeaders(std::vector<std::string> headers);
		void	requestIsWellFormed();
		void	fillRequestLine(const std::string& requestLine);
		void	parseContentLength();
		void	parseContentType();
		void	parseTransferEncoding();
		void	matchUriRequest();

	/* *************************** getters *************************** */
		std::string getBody();
		std::map<std::string, std::string> getRequestLine();
		std::map<std::string, std::string> getHeaders();
		int getStatus();

};
