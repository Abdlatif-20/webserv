/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 21:57:14 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/03 00:21:43 by aben-nei         ###   ########.fr       */
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


enum RequestError
{
	NotImplemented = 501,
	BadRequest = 400,
	RequestURITooLong = 414,
	RequestEntityTooLarge = 413,
	NotFound = 404,
	MovedPermanently = 301,
	MethodNotAllowed = 405,
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
		static std::map<std::string, std::string> _headers;
		static std::string _body;
		static std::string _method;
		static std::string _path;
		static std::string _version;
		static int			_status;
		Request();
	public:
		~Request();
	
	/* *************************** methods *************************** */
		static void	parseRequest(std::string buffer);
		static void fillHeaders(std::vector<std::string> headers);
		static void	checkRequirements(std::string buffer);
		static void	is_req_well_formed();

	/* *************************** getters *************************** */
		static std::string getMethod();
		static std::string getPath();
		static std::string getVersion();
		static std::string getBody();
		static std::map<std::string, std::string> getHeaders();
		static int getStatus();


};
