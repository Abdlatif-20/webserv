/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:22 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/18 02:37:39 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, std::string> Response::reasonPhrases;
std::map<std::string, std::string> Response::mimeTypes;

Response::Response()
{
    request = NULL;
    context = NULL;
    statusCode = 200;
    responseDone = false;
    headersSent = false;
    std::memset(buffer, 0, sizeof(buffer));
    fd = INT_MIN;
    responseDone = false;
    isWorking = false;
    isRedirection = false;
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
    std::memcpy(buffer, obj.buffer, sizeof(buffer));
    fd = obj.fd;
    responseDone = obj.responseDone;
    isWorking = obj.isWorking;
    isRedirection = obj.isRedirection;
    location = obj.location;
    return *this;
}

Response::~Response()
{
	close(fd);
	fd = INT_MIN;
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
    return "<!DOCTYPE html><html><body style='text-align: center;'><h2>Error "
        + Utils::intToString(statusCode) + " - "
        + reasonPhrases[statusCode] + "</h2><hr><h4>WebServer 1.0</h4></body></html>";
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
    headers += "Content-Type: " + (bodyPath.empty() ? "text/html" : getMimeType(Utils::getFileExtension(bodyPath))) + CRLF;
    if (isRedirection)
        headers += "Location: " + location + CRLF;
    headers += CRLF;
}

void Response::prepareBody()
{
    if (bodyPath.empty())
    {
        responseDone = true;
        return;
    }
    if (fd == INT_MIN)
        fd = open(bodyPath.c_str(), O_RDONLY);
    if (fd)
    {
        statusCode = INTERNAL_SERVER_ERROR;
        generateResponseError();
        responseDone = true;
        return;
    }
    ssize_t readedBytes = read(fd, buffer, sizeof(buffer));
    if (readedBytes == -1)
    {
        
    }
    if (readedBytes == 0)
    {
        close(fd);
        responseDone = true;
        return;
    }
    body.clear();
    body.append(buffer, readedBytes);
}

void Response::prepareGET()
{
    if (isWorking)
        return;
    std::string resource = context->getRoot() + request->getRequestPath();
    if (!Utils::checkIfPathExists(resource))
    {
        statusCode = NotFound;
        generateResponseError();
        return;
    }
    if (Utils::isDirectory(resource))
    {
        if (!Utils::stringEndsWith(resource, "/"))
            prepareRedirection(MovedPermanently, request->getRequestPath() + "/");
        else
        {
            try
            {
                std::string index = context->getIndex(resource);
                std::cout << index << std::endl;
                if (index.empty())
                {
                    if (context->getAutoIndex())
                        autoIndex(resource);
                    else
                    {
                        statusCode = FORBIDDEN;
                        generateResponseError();
                    }
                    return;
                }
                bodyPath = resource + index;
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
    }
    else
    {
        if (!Utils::isReadableFile(resource))
        {
            statusCode = FORBIDDEN;
            generateResponseError();
            return;
        }
        bodyPath = resource;
        statusCode = OK;
    }
    isWorking = true;
}

void Response::prepareRedirection(int _status, const std::string& _location)
{
    isRedirection = true;
    statusCode = _status;
    location = _location;
}

void Response::autoIndex(const std::string& path)
{
    struct dirent *entry;
    DIR *dir;
    std::string html = "<!DOCTYPE html><html><head><title>Index of $indexof$</title><link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'><link rel='preconnect' href='https://fonts.googleapis.com'><link rel='preconnect' href='https://fonts.gstatic.com' crossorigin><link href='https://fonts.googleapis.com/css2?family=Madimi+One&family=Manjari:wght@100;400;700&family=Poppins:ital,wght@0,100;0,200;0,300;0,400;0,500;0,600;0,700;0,800;0,900;1,100;1,200;1,300;1,400;1,500;1,600;1,700;1,800;1,900&display=swap' rel='stylesheet'><style>body{font-family: 'Manjari', sans-serif;font-weight: 300;font-style: normal;}table, tr, th, td {border-collapse: collapse;border: 1px solid rgb(186, 186, 186);padding:8px;font-size: 15px;letter-spacing: 1px;text-align: left;}</style></head><body><h1>Index of $indexof$</h1><hr><table style='width: 70%;'><tr><th>Name</th><th>Last Modified</th><th>Size</th><th>Type</th></tr>";
    html = Utils::replaceAll(html, "$indexof$", request->getRequestPath());
    dir = opendir(path.c_str());
    while ((entry = readdir(dir)))
    {
        if (Utils::isDirectory(path + entry->d_name))
            html += "<tr><td><i class='glyphicon glyphicon-folder-close'></i>";
        else
            html += "<tr><td><i class='glyphicon glyphicon-file'></i>";
        html+= "<a style='text-decoration:none'; href='" + std::string(entry->d_name) + "'> " + entry->d_name + "</td>";
        html += "<td>" + Utils::get_last_modified_date(path + entry->d_name) + "</td>";
        html += "<td>" + (Utils::getFileSize(path + entry->d_name) == -1 ? "--" : Utils::longlongToString(Utils::getFileSize(path + entry->d_name))) + "</td>";
        html += "<td>" + (Utils::isDirectory(path + entry->d_name) ? "Directory" : Utils::getFileExtension(path + entry->d_name)) + "</td>";
        html += "</tr>";
    }
    html += "</table></body></html>";
    body = html;
    bodyPath.clear();
    statusCode = OK;
}

void Response::prepareResponse()
{
    /* Handle request Errors */
    if (request->getStatus() >= 400)
    {
        statusCode = request->getStatus();
        generateResponseError();
        prepareHeaders();
        responseDone = true;
        return;
    }
    if (context->getHttpRedirection().size() > 0)
    {
        prepareRedirection(Utils::stringToInt(context->getHttpRedirection().at(0)), context->getHttpRedirection().at(1));
        prepareBody();
        prepareHeaders();
        responseDone = true;
        return;
    }
	if (request->getMethod() == "GET")
    {
        prepareGET();
        prepareBody();
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
    fd = INT_MIN;
    context = NULL;
    request = NULL;
    statusCode = 200;
    headersSent = false;
    responseDone = false;
    isWorking = false;
    isRedirection = false;
    location.clear();
    body.clear();
    bodyPath.clear();
    headers.clear();
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
    reasonPhrases[405] = "Not Allowed";
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