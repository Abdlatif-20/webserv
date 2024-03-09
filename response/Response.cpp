/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:22 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/08 11:01:08 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, std::string> Response::reasonPhrases;
std::map<std::string, std::string> Response::mimeTypes;

Response::Response()
{
    statusCode = 0;
}

Response::Response(const Response &obj)
{
    *this = obj;
}

Response& Response::operator=(const Response& obj)
{
    if (this == &obj)
        return *this;
    this->serverCtx = obj.serverCtx;
    this->request = obj.request;
    this->statusCode = obj.statusCode;
    this->headers = obj.headers;
    this->body = obj.body;
    return *this;
}

Response::~Response()
{

}

void Response::setServerCtx(const ServerContext& serverCtx)
{
    this->serverCtx = serverCtx;
}

void Response::setRequest(const Request& request)
{
    this->request = request;
}

void Response::setStatusCode(int statusCode)
{
    this->statusCode = statusCode;
}

void Response::addHeader(const std::string& key, const std::string& value)
{
    headers[key] = value;
}

void Response::setBody(const std::string& body)
{
    this->body = body;
}

const ServerContext &Response::getServerCtx() const
{
    return serverCtx;
}

const Request &Response::getRequest() const
{
    return request;
}

int Response::getStatusCode() const
{
    return statusCode;
}

const std::map<std::string, std::string>& Response::getHeaders() const
{
    return headers;
}

const std::string &Response::getBody() const
{
    return body;
}

std::string Response::getMimeType(const std::string& extension)
{
    std::map<std::string, std::string>::const_iterator it = mimeTypes.find(extension);
    if (it == mimeTypes.cend())
        return UNKNOWN_MIMETYPE;
    return it->second;
}

std::string Response::generateResponse()
{
    if (!statusCode)
        statusCode = request.getStatus();
    std::string errorPage;
    if (statusCode >= 400)
    {
        headers["Date"] = Utils::getCurrentTime();
        headers["Server"] = "WebServer 1.0";
        headers["Connection"] = "Closed";
        try
        {
            errorPage = serverCtx.getErrorPage(Utils::intToString(statusCode));
            errorPage.empty() ? throw Utils::FileNotFoundException() : errorPage = serverCtx.getRoot() + errorPage;
            body = Utils::readFile(errorPage);
            headers["Content-Type"] = getMimeType(Utils::getFileExtension(errorPage));
            headers["Content-Length"] = body.length();
            return toString();
        }
        catch (const Utils::FileNotFoundException& e)
        {
            if (statusCode != 404)
                return (statusCode = 404, generateResponse());
        }
        catch (const Utils::FilePermissionDenied& e)
        {
            if (statusCode != 403)
                return (statusCode = 403, generateResponse());
        }
        body = generateHtmlErrorPage();
        headers["Content-Type"] = "text/html";
        headers["Content-Length"] = body.length();
    }
    else if (statusCode == 200)
    {
        // std::cout << request.getLocationCtx().getIndex() << std::endl;
    }
    return toString();
}


std::string Response::toString()
{
    // std::string httpVersion = HTTP_VERSION;
    std::string responseText = HTTP_VERSION" " + Utils::intToString(statusCode) + " " + reasonPhrases[statusCode] + CRLF;
    std::map<std::string, std::string>::const_iterator it = headers.cbegin();
    while (it != headers.cend())
        responseText += it->first + ": " + (it++)->second + CRLF;
    responseText += CRLF"abcd";
    return responseText;
}

std::string Response::generateHtmlErrorPage()
{
    return "<!DOCTYPE html><html><body><h1 style='text-align: center;'>Error "
        + Utils::intToString(statusCode) + " - "
        + reasonPhrases[statusCode] + "</body></html>";
}

void Response::initReasonPhrases()
{
    /* Informational */
    reasonPhrases[200] = "OK";
    reasonPhrases[201] = "Created";
    reasonPhrases[202] = "Accepted";
    reasonPhrases[203] = "Non-Authoritative Information";
    reasonPhrases[204] = "No Content";
    reasonPhrases[205] = "Reset Content";
    reasonPhrases[206] = "Partial Content";

    /* Successful but Requires Further Action */
    reasonPhrases[300] = "Multiple Choices";
    reasonPhrases[301] = "Moved Permanently";
    reasonPhrases[302] = "Found";
    reasonPhrases[303] = "See Other";
    reasonPhrases[304] = "Not Modified";
    reasonPhrases[305] = "Use Proxy";
    reasonPhrases[307] = "Temporary Redirect";

    /* Client Error */
    reasonPhrases[400] = "Bad Request";
    reasonPhrases[401] = "Unauthorized";
    reasonPhrases[402] = "Payment Required";
    reasonPhrases[403] = "Forbidden";
    reasonPhrases[404] = "Not Found";
    reasonPhrases[405] = "Method Not Allowed";
    reasonPhrases[406] = "Not Acceptable";
    reasonPhrases[407] = "Proxy Authentication Required";
    reasonPhrases[408] = "Request Timeout";
    reasonPhrases[409] = "Conflict";
    reasonPhrases[410] = "Gone";
    reasonPhrases[411] = "Length Required";
    reasonPhrases[412] = "Precondition Failed";
    reasonPhrases[413] = "Payload Too Large";
    reasonPhrases[414] = "URI Too Long";
    reasonPhrases[415] = "Unsupported Media Type";
    reasonPhrases[416] = "Requested Range Not Satisfiable";
    reasonPhrases[417] = "Expectation Failed";

    /* Server Error */
    reasonPhrases[500] = "Internal Server Error";
    reasonPhrases[501] = "Not Implemented";
    reasonPhrases[502] = "Bad Gateway";
    reasonPhrases[503] = "Service Unavailable";
    reasonPhrases[504] = "Gateway Time-out";
    reasonPhrases[505] = "HTTP Version not supported";
}

void Response::initMimeTypes()
{
    /* Text Types */
    mimeTypes[".html"] = "text/html";
    mimeTypes[".htm"] = "text/html";
    mimeTypes[".php"] = "application/x-httpd-php";
    mimeTypes[".css"] = "text/css";
    mimeTypes[".txt"] = "text/plain";
    mimeTypes[".csv"] = "text/csv";
    mimeTypes[".ics"] = "text/calendar";
    mimeTypes[".js"] = "application/javascript";
    mimeTypes[".json"] = "application/json";

    /* Image Types */
    mimeTypes[".jpg"] = "image/jpeg";
    mimeTypes[".jpeg"] = "image/jpeg";
    mimeTypes[".png"] = "image/png";
    mimeTypes[".gif"] = "image/gif";
    mimeTypes[".bmp"] = "image/bmp";
    mimeTypes[".svg"] = "image/svg+xml";
    mimeTypes[".ico"] = "image/vnd.microsoft.icon";
    mimeTypes[".webp"] = "image/webp";

    /*Audio Types*/
    mimeTypes[".mp3"] = "audio/mpeg";
    mimeTypes[".aac"] = "audio/aac";
    mimeTypes[".oga"] = "audio/ogg";
    mimeTypes[".flac"] = "audio/flac";
    mimeTypes[".wav"] = "audio/vnd.wave";
    mimeTypes[".midi"] = "audio/midi";
    mimeTypes[".mid"] = "audio/midi";

    /*Video Types*/
    mimeTypes[".mp4"] = "audio/mpeg";
    mimeTypes[".webm"] = "video/webm";
    mimeTypes[".ogv"] = "video/ogg";
    mimeTypes[".avi"] = "video/x-msvideo";
    mimeTypes[".mov"] = "video/quicktime";
    mimeTypes[".wmv"] = "video/x-ms-wmv";
    mimeTypes[".flv"] = "video/x-flv";

    /*Application Types*/
    mimeTypes[".pdf"] = "application/pdf";
    mimeTypes[".doc"] = "application/msword";
    mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mimeTypes[".xls"] = "application/vnd.ms-excel";
    mimeTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
    mimeTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mimeTypes[".zip"] = "application/zip";
    mimeTypes[".rar"] = "application/vnd.rar";
    mimeTypes[".gz"] = "application/gzip";
    mimeTypes[".bz2"] = "application/x-bzip2";
    mimeTypes[".7z"] = "application/x-7z-compressed";
    mimeTypes[".xml"] = "application/xml";
}