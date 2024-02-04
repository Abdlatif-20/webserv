/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/04 13:23:58 by aben-nei         ###   ########.fr       */
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


enum RequestError
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

class Request
{
	private:
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _method;
		std::string _path;
		std::string _version;
		int			_status;
		Request();
	public:
		~Request();

	/* *************************** exceptions *************************** */
		class InvalidPathException : public std::exception
		{
			virtual const char* what() const throw();
		};

		class InvalidMethodException : public std::exception
		{
			virtual const char* what() const throw();
		};

		class InvalidVersionException : public std::exception
		{
			virtual const char* what() const throw();
		};

		class InvalidHeaderException : public std::exception
		{
			virtual const char* what() const throw();
		};

		class InvalidBodyException : public std::exception
		{
			virtual const char* what() const throw();
		};

		class InvalidRequestException : public std::exception
		{
			virtual const char* what() const throw();
		};
	
	/* *************************** methods *************************** */
		void	parseRequest(std::string buffer);
		void fillHeaders(std::vector<std::string> headers);
		// void	fillBuddy(std::string req);
		void	checkRequirements(std::string buffer);
		void	is_req_well_formed();
		void	handelTransferEncoding();
		// void	handelContentLength();
		// void	handelContentType();

	/* *************************** getters *************************** */
		std::string getMethod();
		std::string getPath();
		std::string getVersion();
		std::string getBody();
		std::map<std::string, std::string> getHeaders();
		int getStatus();

};
