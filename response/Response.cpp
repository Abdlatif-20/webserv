/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:22 by mel-yous          #+#    #+#             */
/*   Updated: 2024/04/26 19:14:46 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<int, std::string> Response::reasonPhrases;
std::map<std::string, std::string> Response::mimeTypes;

Response::Response()
{
    request = NULL;
    statusCode = 200;
    responseDone = false;
    headersSent = false;
    std::memset(buffer, 0, sizeof(buffer));
    isWorking = false;
    isRedirection = false;
    isRanged = false;
    startOffset = 0;
    endOffset = 0;
    alreadySeeked = false;
    sendedBytes = 0;
    readSize = sizeof(buffer);
	CGIWorking = false;
}

Response::Response(const Response &obj)
{
    *this = obj;
}

Response& Response::operator=(const Response& obj)
{
    if (this == &obj)
        return *this;
	env = obj.env;
	CGIWorking = obj.CGIWorking;
    request = obj.request;
    serverCTX = obj.serverCTX;
    locationCTX = obj.locationCTX;
    statusCode = obj.statusCode;
    responseDone = obj.responseDone;
    headers = obj.headers;
    headersSent = obj.headersSent;
    bodyPath = obj.bodyPath;
    std::memcpy(buffer, obj.buffer, sizeof(buffer));
    isWorking = obj.isWorking;
    isRedirection = obj.isRedirection;
    location = obj.location;
    isRanged = obj.isRanged;
    startOffset = obj.startOffset;
    endOffset = obj.endOffset;
    alreadySeeked = obj.alreadySeeked;
    sendedBytes = obj.sendedBytes;
    readSize = obj.readSize;
    return *this;
}

Response::~Response()
{
    ifs.close();
}

void Response::setRequest(Request* request)
{
    this->request = request;
}

void Response::setServerCTX(const ServerContext& serverCTX)
{
    this->serverCTX = serverCTX;
}

void Response::setLocationCTX(const LocationContext& locationCTX)
{
    this->locationCTX= locationCTX;
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

std::map<std::string, std::string>& Response::getHeaders()
{
    return headers;
}

const std::string& Response::getHeaderByName(const std::string& name)
{
    return headers[name];
}

bool Response::getHeadersSent() const
{
    return headersSent;
}

bool Response::responseIsDone() const
{
    return responseDone;
}

std::string Response::generateHtmlResponsePage()
{
    return Utils::replaceAll(HTML_RESPONSE_PAGE, "$title$", reasonPhrases[statusCode]) + Utils::numberToString(statusCode) + " - "
        + reasonPhrases[statusCode] + "</h2><hr><h4>WebServer 1.0</h4></body></html>\n";
}

void Response::generateResponseError()
{
    Label:
    std::string errorPage = locationCTX.getErrorPage(Utils::numberToString(statusCode));
    if (errorPage.empty())
    {
        bodyPath.clear();
        body = generateHtmlResponsePage();
    }
    else
    {
        std::string path = locationCTX.getRoot() + errorPage;
        if (!Utils::checkIfPathExists(path))
        {
            if (statusCode == NotFound)
                return (bodyPath.clear(), body = generateHtmlResponsePage(), void());
            statusCode = NotFound;
            goto Label;
        }
        if (Utils::isDirectory(path) || !Utils::isReadableFile(path))
        {
            if (statusCode == FORBIDDEN)
                return (bodyPath.clear(), body = generateHtmlResponsePage(), void());
            statusCode = FORBIDDEN;
            goto Label;
        }
        bodyPath = path;
    }
}

void Response::prepareHeaders()
{
    if (headersSent)
        return;
	if (statusLine.empty())
		statusLine = std::string(HTTP_VERSION) + SPACE + Utils::numberToString(statusCode) + SPACE + reasonPhrases[statusCode] + CRLF;
    setHeaderAttr("connection", request->getHeaderByName("connection"));
    setHeaderAttr("server", std::string(SERVER) + " (" + OS_MAC + ")");
    setHeaderAttr("date", Utils::getCurrentTime());
    setHeaderAttr("content-length", (bodyPath.empty() ? Utils::numberToString(body.size()) : Utils::numberToString(Utils::getFileSize(bodyPath))));
    setHeaderAttr("content-type", (bodyPath.empty() ? "text/html" : getMimeType(Utils::getFileExtension(bodyPath))));
    if (!bodyPath.empty())
        setHeaderAttr("last-modified", Utils::get_last_modified_date(bodyPath));
    if (isRedirection)
        setHeaderAttr("location", location);
    if (isRanged)
    {
        setHeaderAttr("accept-ranges", "bytes");
        setHeaderAttr("content-length", Utils::numberToString((endOffset - startOffset) + 1));
        setHeaderAttr("content-range", "bytes " + Utils::numberToString(startOffset) + "-" + Utils::numberToString(endOffset) + "/" + Utils::numberToString(Utils::getFileSize(bodyPath)));
    }
}



void Response::prepareBody()
{
	if (statusCode == 200 && locationCTX.hasCGI(bodyPath) && !CGIWorking)
    {
		runCGI();
    }
    if (bodyPath.empty())
    {
        responseDone = true;
        return;
    }
    if (!ifs.is_open())
    {
        ifs.open(bodyPath);
        if (!ifs.is_open())
            throw ResponseErrorException(InternalServerError);
    }
    std::memset(buffer, 0, sizeof(buffer));
    if (isRanged)
    {
        if (!alreadySeeked)
        {
            ifs.seekg(startOffset, ifs.beg);
            alreadySeeked = true;
        }
        size_t contentLength = endOffset - startOffset;
        if (contentLength <= sizeof(buffer))
            readSize = contentLength;
    }
    ssize_t readedBytes = myRead(ifs, buffer, readSize);
    if (readedBytes == -1)
        throw ResponseErrorException(InternalServerError);
    if (!readedBytes || (isRanged && sendedBytes >= endOffset - startOffset))
    {
        body.clear();
        ifs.close();
        responseDone = true;
        return;
    }
    body.clear();
    body.append(buffer, readSize);
    sendedBytes += readedBytes;
}

void Response::prepareGET()
{
    if (isWorking)
        return;
    std::string resource = locationCTX.getRoot() + Utils::urlDecoding(request->getRequestPath());
    if (!Utils::checkIfPathExists(resource))
        throw ResponseErrorException(NotFound);
    if (Utils::isDirectory(resource))
    {
        if (!Utils::stringEndsWith(resource, "/"))
            prepareRedirection(MovedPermanently, request->getRequestPath() + "/");
        else
        {
            try
            {
                std::string index = locationCTX.getIndex(resource);
                if (index.empty())
                {
                    if (locationCTX.getAutoIndex())
                        autoIndex(resource);
                    else
                        throw ResponseErrorException(FORBIDDEN);
                    return;
                }
                bodyPath = resource + index;
                statusCode = OK;
            }
            catch (const Utils::FilePermissionDenied& e)
            {
                throw ResponseErrorException(FORBIDDEN);
            }
            catch (const Utils::FileNotFoundException& e)
            {
                throw ResponseErrorException(NotFound);
            }
        }
    }
    else
    {
        if (!Utils::isReadableFile(resource))
            throw ResponseErrorException(FORBIDDEN);
        bodyPath = resource;
        statusCode = OK;
    }
    prepareRanged();
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
    std::string html = Utils::replaceAll(AUTO_INDEX_TEMPLATE, "$indexof$", request->getRequestPath());
    dir = opendir(path.c_str());
    if (!dir)
        throw ResponseErrorException(FORBIDDEN);
    while ((entry = readdir(dir)))
    {
        if (Utils::isDirectory(path + entry->d_name))
            html += "<tr><td><i class='glyphicon glyphicon-folder-close'></i>";
        else
            html += "<tr><td><i class='glyphicon glyphicon-file'></i>";
        html+= "<a style='text-decoration:none'; href='" + Utils::urlEncoding(std::string(entry->d_name)) + "'> " + entry->d_name + "</td>";
        html += "<td>" + Utils::get_last_modified_date(path + entry->d_name) + "</td>";
        html += "<td>" + (Utils::getFileSize(path + entry->d_name) == -1 ? "--" : Utils::bytesToHuman(Utils::getFileSize(path + entry->d_name))) + "</td>";
        html += "<td>" + (Utils::isDirectory(path + entry->d_name) ? "Directory" : Utils::getFileExtension(path + entry->d_name)) + "</td>";
        html += "</tr>";
    }
    closedir(dir);
    html += "</table></body></html>";
    body = html; 
    bodyPath.clear();
    statusCode = OK;
}

void Response::runCGI()
{
	CGI cgi(this, request);

	cgi.setupEnv(bodyPath);
	cgi.execute();
	CGIWorking = true;
}

void Response::prepareRanged()
{
    std::string rangeHeader = request->getHeaderByName("Range");
    if (!rangeHeader.empty())
    {
        if (getRangeUnit(rangeHeader) == "bytes")
        {
            std::string range = getRangeBytes(rangeHeader);
            ssize_t contentLength = Utils::getFileSize(bodyPath);
            if (range.empty())
                return;
            isRanged = true;
            startOffset = getStartOffset(range);
            endOffset = getEndOffset(range);
            if (endOffset == -1 || endOffset >= contentLength)
                endOffset = contentLength - 1;
            if (startOffset > contentLength || endOffset < startOffset)
                    throw ResponseErrorException(RequestedRangeNotSatisfiable);
            statusCode = PartialContent;
        }
    }
}

void Response::preparePOST()
{
    if (!locationCTX.getUploadStore().empty())
    {
        statusCode = Created;
        body = generateHtmlResponsePage();
        bodyPath.clear();
    }
    else
    {
        std::string resource = locationCTX.getRoot() + Utils::urlDecoding(request->getRequestPath());
        if (!Utils::checkIfPathExists(resource))
            throw ResponseErrorException(NotFound);
        if (Utils::isDirectory(resource))
        {
            if (!Utils::stringEndsWith(resource, "/"))
                prepareRedirection(MovedPermanently, request->getRequestPath() + "/");
            else
            {
                try
                {
                    std::string index = locationCTX.getIndex(resource);
                    if (index.empty())
                        throw Utils::FilePermissionDenied();
                    /* Request BODY goes to CGI !! */
					if (!locationCTX.hasCGI(index))
						headers["content-type"] = "text/plain";
					bodyPath = resource + index;
                	statusCode = OK;
                }
                catch (const std::exception& e)
                {
                    throw ResponseErrorException(FORBIDDEN);
                }
            }
        }
        else
        {
            if (!locationCTX.hasCGI(resource))
				headers["content-type"] = "text/plain";
			bodyPath = resource;
            statusCode = OK;
        }
    }
}

void Response::prepareDELETE()
{
        std::string resource = locationCTX.getRoot() + Utils::urlDecoding(request->getRequestPath());
        if (!Utils::checkIfPathExists(resource))
            throw ResponseErrorException(NotFound);
        if (Utils::isDirectory(resource))
        {
            if (!Utils::stringEndsWith(resource, "/"))
                prepareRedirection(MovedPermanently, request->getRequestPath() + "/");
            else
            {
                try
                {
                    std::string index = locationCTX.getIndex(resource);
                    if (index.empty() )
                        throw Utils::FilePermissionDenied();
                    /* Request BODY goes to CGI !! */
					if (!locationCTX.hasCGI(index))
						headers["content-type"] = "text/plain";
					bodyPath = resource + index;
                	statusCode = OK;
                }
                catch (const std::exception& e)
                {
                    throw ResponseErrorException(FORBIDDEN);
                }
            }
        }
        else
        {
            if (!locationCTX.hasCGI(resource))
				headers["content-type"] = "text/plain";
			bodyPath = resource;
            statusCode = OK;
        }
}

void Response::prepareResponse()
{
    try
    {
        if (request->getStatus() >= 400)
            throw ResponseErrorException(request->getStatus());
        if (locationCTX.getHttpRedirection().size() > 0)
        {
            prepareRedirection(Utils::stringToInt(locationCTX.getHttpRedirection()[0]), locationCTX.getHttpRedirection()[1]);
            prepareBody();
            prepareHeaders();
            responseDone = true;
            return;
        }
        if (request->getMethod() == "GET")
            prepareGET();
        else if (request->getMethod() == "POST")
            preparePOST();
        else if (request->getMethod() == "DELETE")
            prepareDELETE();
        else
            throw ResponseErrorException(NotImplemented);
        prepareBody();
        prepareHeaders();
    }
    catch (const ResponseErrorException& e)
    {
        isRanged = false;
        statusCode = e.status;
        generateResponseError();
        prepareBody();
        prepareHeaders();
    }
	catch (...)
	{
		isRanged = false;
		statusCode = InternalServerError;
		generateResponseError();
		prepareBody();
		prepareHeaders();
	}
}

void Response::setStatusLine(std::string val)
{
	statusLine = val;
}

void Response::setHeaderAttr(std::string key, std::string value)
{
	Utils::toLower(key);
	if (headers[key].empty())
		headers[key] = value;
}

void Response::resetResponse()
{
    request = NULL;
    statusCode = 200;
    isWorking = false;
	CGIWorking = false;
    headersSent = false;
    responseDone = false;
    isRedirection = false;
	env.clear();
    body.clear();
    headers.clear();
    bodyPath.clear();
    location.clear();
    isRanged = false;
    startOffset = 0;
    endOffset = 0;
    alreadySeeked = false;
    sendedBytes = 0;
    readSize = sizeof(buffer);
    ifs.close();
}

std::string Response::headersToString()
{
    std::map<std::string, std::string>::iterator it = headers.begin();
    std::string headers_str = statusLine;
    while (it != headers.end())
    {
        headers_str += (it->first + ": ") + it->second + CRLF;
        it++;
    }
    headers_str += CRLF;
    return headers_str;
}

std::ifstream &Response::getIfs()
{
	return (ifs);
}

ServerContext &Response::getServerCtx()
{
	return (serverCTX);
}

LocationContext &Response::getLocationCtx()
{
	return (locationCTX);
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
    reasonPhrases[413] = "Content Too Large";
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
    reasonPhrases[505] = "HTTP Version Not Supported";
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