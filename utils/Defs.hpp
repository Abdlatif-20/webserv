/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:36:15 by mel-yous          #+#    #+#             */
/*   Updated: 2024/05/01 13:11:20 by mel-yous         ###   ########.fr       */
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
#define AUTO_INDEX_TEMPLATE "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Index of $indexof$</title><link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'><link rel='preconnect' href='https://fonts.googleapis.com'><link rel='preconnect' href='https://fonts.gstatic.com' crossorigin><link href='https://fonts.googleapis.com/css2?family=Madimi+One&family=Manjari:wght@100;400;700&family=Poppins:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap' rel='stylesheet'><style>body{font-family: 'M PLUS 1p', sans-serif;font-weight: 400;font-style: normal;}table, tr, th, td {border-collapse: collapse;border: 0px solid rgb(186, 186, 186);padding:10px;font-size: 14px;letter-spacing: 1px;text-align: left;}</style></head><body><h1>Index of $indexof$</h1><hr><table style='width: 70%;'><tr><th>Name</th><th>Last Modified</th><th>Size</th><th>Type</th></tr>"
#define HTML_RESPONSE_PAGE "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>$title$</title><link rel='preconnect' href='https://fonts.googleapis.com'><link rel='preconnect' href='https://fonts.gstatic.com' crossorigin><link href='https://fonts.googleapis.com/css2?family=M+PLUS+1p&display=swap' rel='stylesheet'><style>body{font-family: 'M PLUS 1p', sans-serif;font-weight: 400;font-size:13px;font-style: normal;text-align: center;}</style></head><body><h2>"

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
    CGI_ASSIGN,
    CGI_MAX_TIMEOUT,
    UNKNOWN
} t_directive;

enum StatusCodes
{
	OK = 200,
    Created = 201,
	NotFound = 404,
    NoContent = 204,
    FORBIDDEN = 403,
    BadGateway = 502,
	BadRequest = 400,
	GetwayTimeout = 504,
	RequestTimeout = 408,
	LengthRequired = 411,
    PartialContent = 206,
	NotImplemented = 501,
	MovedPermanently = 301,
	MethodNotAllowed = 405,
	RequestURITooLong = 414,
	RequestEntityTooLarge = 413,
    InternalServerError = 500,
	HTTPVersionNotSupported = 505,
    RequestedRangeNotSatisfiable = 416
};