/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:36:15 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/20 16:49:01 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <utility>
#include <map>
#include <iostream>

class ServerContext;
class Token;
class Server;
class LocationContext;

#define HTTP_VERSION "HTTP/1.1"
#define SERVER "WebServer 1.0"
#define UNKNOWN_MIMETYPE "application/octet-stream"
#define SPACE " "

#ifdef _WIN32
#define OS_WINDOWS "Windows"

#elif defined(__APPLE__)
#define OS_MAC "macOS"

#elif defined(__linux__)
#define OS_LINUX "Linux"

#elif defined(__unix__) || defined(__posix__)
#define OS_UNIX "Unix"

#else
#define OS_UNKNOWN "Unknown"
#endif

typedef std::vector<std::string> StringVector;
typedef std::pair<std::string, StringVector> Directive;
typedef std::map<std::string, StringVector>  DirectivesMap;
typedef std::vector<ServerContext> ServersVector;
typedef std::vector<Token> TokensVector;
typedef std::vector<LocationContext> LocationsVector;
typedef std::invalid_argument	Fail;
typedef std::vector<Server>::iterator	servers_it;

typedef enum e_directive
{
    ROOT,
    INDEX,
    AUTO_INDEX,
    ERROR_PAGE,
    CLIENT_MAX_BODY_SIZE,
    ALLOWED_METHODS,
    LISTEN,
    SERVER_NAME,
    RETURN,
    LOCATION,
    UPLOAD_STORE,
    UNKNOWN
} t_directive;

enum StatusCodes
{
	OK = 200,
	NotFound = 404,
    FORBIDDEN = 403,
	BadRequest = 400,
	LengthRequired = 411,
	NotImplemented = 501,
	MovedPermanently = 301,
	MethodNotAllowed = 405,
	RequestURITooLong = 414,
	RequestEntityTooLarge = 413,
    InternalServerError = 500,
	HTTPVersionNotSupported = 505
};