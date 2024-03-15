/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:22 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/15 14:16:57 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, std::string> Response::reasonPhrases;
std::map<std::string, std::string> Response::mimeTypes;

Response::Response()
{
    request = NULL;
    context = NULL;
    statusCode = 0;
    responseDone = false;
    headersSent = false;
    std::memset(buffer, 0, sizeof(buffer));
    fd = INT_MIN;
    responseDone = false;
}

Response::Response(const Response &obj)
{
    *this = obj;
}

Response& Response::operator=(const Response& obj)
{
    if (this == &obj)
        return *this;
    request = obj.request;
    context = obj.context;
    statusCode = obj.statusCode;
    responseDone = obj.responseDone;
    headers = obj.headers;
    headersSent = obj.headersSent;
    bodyPath = obj.bodyPath;
    std::memcpy(buffer, obj.buffer, sizeof(obj.buffer));
    fd = obj.fd;
    responseDone = obj.responseDone;
    return *this;
}

Response::~Response()
{

}

void Response::setRequest(Request* request)
{
    this->request = request;
}

void Response::setContext(Context* context)
{
    this->context = context;
}

void Response::setHeadersSent(bool flag)
{
    headersSent = flag;
}

std::string Response::getMimeType(const std::string& extension)
{
    std::map<std::string, std::string>::iterator it = mimeTypes.find(extension);
    if (it == mimeTypes.end())
        return UNKNOWN_MIMETYPE;
    return it->second;
}

const std::string& Response::getBody() const
{
    return body;
}

const std::string& Response::getHeaders() const
{
    return headers;
}

bool Response::getHeadersSent() const
{
    return headersSent;
}

bool Response::responseIsDone() const
{
    return responseDone;
}

std::string Response::generateHtmlErrorPage()
{
    return "<!DOCTYPE html><html><body><h1 style='text-align: center;'>Error "
        + Utils::intToString(statusCode) + " - "
        + reasonPhrases[statusCode] + "</body></html>";
}

bool Response::checkErrorPage(const std::string& path)
{
    if (!Utils::checkIfPathExists(path))
    {
        bodyPath.clear();
        statusCode = NotFound;
        body = generateHtmlErrorPage();
        return false;
    }
    else if (Utils::isDirectory(path) || !Utils::isReadableFile(path))
    {
        bodyPath.clear();
        statusCode = FORBIDDEN;
        body = generateHtmlErrorPage();
        return false;
    }
    return true;
}

void Response::generateResponseError()
{
    std::string errorPage = context->getErrorPage(Utils::intToString(statusCode));
    if (errorPage.empty())
    {
        bodyPath.clear();
        body = generateHtmlErrorPage();
    }
    else
    {
        std::string path = context->getRoot() + errorPage;
        if (!checkErrorPage(path))
            return;
        bodyPath = path;
    }
}

void Response::prepareHeaders()
{
    if (headersSent)
        return;
    headers += std::string(HTTP_VERSION) + SPACE + Utils::intToString(statusCode) + SPACE + reasonPhrases[statusCode] + CRLF;
    headers += "Connection: " + request->getHeaderByName("connection") + CRLF;
    headers += "Server: " + std::string(SERVER) + CRLF;
    headers += "Date: " + Utils::getCurrentTime() + CRLF;
    headers += "Content-Length: " + (bodyPath.empty() ? Utils::intToString(body.size()) : Utils::longlongToString(Utils::getFileSize(bodyPath))) + CRLF;
    headers += std::string("Accept-Ranges: bytes") + CRLF;
    headers += "Content-Type: " + (bodyPath.empty() ? headers += "text/html" : getMimeType(Utils::getFileExtension(bodyPath))) + CRLF;
    headers += CRLF;
}

void Response::prepareGETBody()
{
    if (bodyPath.empty())
    {
        responseDone = true;
        return;
    }
    if (fd == INT_MIN)
        fd = open(bodyPath.c_str(), O_RDONLY);
    if (fd == -1)
    {
        /*INTERRNAL SERVER ERROR 500*/
        /* ERROR WHILE OPENING FILE TO BE HANDLED LATER */
    }
    ssize_t readedBytes = read(fd, buffer, sizeof(buffer));
    if (readedBytes <= 0)
	{
        responseDone = true;
        return;
    } // to be handled later
    body.append(buffer, readedBytes);
}

void Response::prepareGET()
{
    std::string requestedResource = context->getRoot() + request->getRequestPath();
    if (!Utils::checkIfPathExists(requestedResource))
    {
        statusCode = NotFound;
        generateResponseError();
        return;
    }
    if (Utils::isDirectory(requestedResource))
    {
        try
        {
            std::string index = context->getIndex(requestedResource);
            if (index.empty())
            {
                if (context->getAutoIndex())
                {
                    /* AUTO_INDEX CODE HERE ! */
                }
                else
                {
                    statusCode = FORBIDDEN;
                    generateResponseError();
                }
                return;
            }
            bodyPath = requestedResource + index;
            statusCode = OK;
        }
        catch (const Utils::FilePermissionDenied& e)
        {
            statusCode = FORBIDDEN;
            generateResponseError();
        }
        catch (const Utils::FileNotFoundException& e)
        {
            statusCode = NotFound;
            generateResponseError();
        }
    }
    else
    {
        if (!Utils::isReadableFile(requestedResource))
        {
            statusCode = FORBIDDEN;
            generateResponseError();
            return;
        }
        bodyPath = requestedResource;
    }
}

void Response::prepareResponse()
{
	if (request->getMethod() == "GET")
    {
        prepareGET();
        prepareGETBody();
        prepareHeaders();
    }
	else if (request->getMethod() == "POST")
    {
        /* Prepare POST */
    }
	else if (request->getMethod() == "DELETE")
    {
        /* Prepare DELETE */
    }
    else
    {
        
    }
}

void Response::resetResponse()
{
    request = NULL;
    context = NULL;
    std::memset(buffer, 0, sizeof(buffer));
    fd = INT_MIN;
    statusCode = 0;
    headers.clear();
    body.clear();
    bodyPath.clear();
    headersSent = false;
    responseDone = false;
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