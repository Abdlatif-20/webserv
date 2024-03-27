/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:22 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/27 16:16:43 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string	Response::PATH;
char	**Response::env_ptr;
std::map<int, std::string> Response::reasonPhrases;
std::map<std::string, std::string> Response::mimeTypes;

Response::Response()
{
    request = NULL;
    statusCode = 200;
    responseDone = false;
    headersSent = false;
    std::memset(buffer, 0, sizeof(buffer));
    fd = INT_MIN;
    isWorking = false;
    isRedirection = false;
    hasCGI = false;
    isPartialContent = false;
    startOffset = 0;
    endOffset = 0;
    alreadySeeked = false;
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
    request = obj.request;
    serverCTX = obj.serverCTX;
    locationCTX = obj.locationCTX;
    statusCode = obj.statusCode;
    responseDone = obj.responseDone;
    headers = obj.headers;
    headersSent = obj.headersSent;
    bodyPath = obj.bodyPath;
    std::memcpy(buffer, obj.buffer, sizeof(buffer));
    fd = obj.fd;
    isWorking = obj.isWorking;
    isRedirection = obj.isRedirection;
    location = obj.location;
    hasCGI = obj.hasCGI;
    isPartialContent = obj.isPartialContent;
    startOffset = obj.startOffset;
    endOffset = obj.endOffset;
    alreadySeeked = obj.alreadySeeked;
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

const std::map<std::string, std::string>& Response::getHeaders() const
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

std::string Response::generateHtmlErrorPage()
{
    return HTML_RESPONSE_PAGE + Utils::intToString(statusCode) + " - "
        + reasonPhrases[statusCode]+ "</h2><hr><h4>WebServer 1.0</h4></body></html>";
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
    if (Utils::isDirectory(path) || !Utils::isReadableFile(path))
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
    std::string errorPage = locationCTX.getErrorPage(Utils::intToString(statusCode));
    if (errorPage.empty())
    {
        bodyPath.clear();
        body = generateHtmlErrorPage();
    }
    else
    {
        std::string path = locationCTX.getRoot() + errorPage;
        if (!checkErrorPage(path))
            return;
        bodyPath = path;
    }
}

void Response::prepareHeaders()
{
    if (headersSent)
        return;
    statusLine = std::string(HTTP_VERSION) + SPACE + Utils::intToString(statusCode) + SPACE + reasonPhrases[statusCode] + CRLF;
    headers["Connection"] = request->getHeaderByName("connection");
    headers["Server"] = std::string(SERVER) + " (" + OS_MAC + ")";
    headers["Date"] = Utils::getCurrentTime();
    headers["Content-Length"] = (bodyPath.empty() ? Utils::intToString(body.size()) : Utils::longlongToString(Utils::getFileSize(bodyPath)));
    headers["Accept-Ranges"] = "bytes";
    headers["Content-Type"] = (bodyPath.empty() ? "text/html" : getMimeType(Utils::getFileExtension(bodyPath)));
    if (isRedirection)
        headers["Location"] = location;
    if (isPartialContent)
    {
        headers["Content-Length"] = Utils::longlongToString((endOffset - startOffset) + 1);
        headers["Content-Range"] = "bytes " + Utils::longlongToString(startOffset) + "-" + Utils::longlongToString(endOffset) + "/" + Utils::longlongToString(Utils::getFileSize(bodyPath));
    }
    // headers += "Connection: " + request->getHeaderByName("connection") + CRLF;
    // headers += "Server: " + std::string(SERVER) + " (" + OS_MAC + ")" + CRLF;
    // headers += "Date: " + Utils::getCurrentTime() + CRLF;
    // headers += "Content-Length: " + (bodyPath.empty() ? Utils::intToString(body.size()) : Utils::longlongToString(Utils::getFileSize(bodyPath))) + CRLF;
    // headers += std::string("Accept-Ranges: bytes") + CRLF;
    // headers += "Content-Type: " + (bodyPath.empty() ? "text/html" : getMimeType(Utils::getFileExtension(bodyPath))) + CRLF;
    // if (isRedirection)
    //     headers += "Location: " + location + CRLF;
    // headers += CRLF;
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
    if (fd == -1)
        throw ResponseErrorException(*this, InternalServerError);
    std::memset(buffer, 0, sizeof(buffer));
    if (!alreadySeeked)
    {
        lseek(fd, startOffset, SEEK_SET);
        alreadySeeked = true;
    }
    ssize_t readedBytes = read(fd, buffer, sizeof(buffer));
    if (readedBytes == -1)
    {
        close(fd);
        statusCode = OK;
        responseDone = true;
        return;
    }
    if (readedBytes == 0)
    {
        close(fd);
        responseDone = true;
        return;
    }
    body.clear();
    body.append(buffer, readedBytes);
	if ((size_t) readedBytes < sizeof(buffer))
		responseDone = true;
}

// void g(int s)
// {
// 	if (s == SIGSEGV)
// 		std::cout << "segv"  << std::endl;
// }

// void Response::prepareCGI()
// {
// 	size_t	idx;
// 	std::string	ext;
// 	std::string	line;
// 	std::vector<char*> ee;
// 	std::stringstream ss(PATH);
// 	Map cgi = locationCTX.getCGI();

// 	signal(SIGSEGV, g);
// 	idx = bodyPath.find_last_of(".");
// 	ext = bodyPath.substr(idx);
// 	if (idx == std::string::npos || cgi[ext].empty())
// 		return ;
// 	for (size_t i = 0; i < env.size(); i++)
// 		ee.push_back((char *)env[i].c_str());
// 	ee.push_back(NULL);
// 	while(std::getline(ss, line,':'))
// 	{
// 		line+= "/" + cgi[ext];
// 		if (!access(line.c_str(), F_OK | X_OK))
// 		{
// 			std::cout << line << std::endl;
// 			std::cout << bodyPath << std::endl;
			
// 			char *args[3] = {(char *)line.c_str(), (char *)bodyPath.c_str(), NULL};
// 			std::string name = "/tmp/output_" ;
// 			name += Utils::intToString(std::rand());
// 			name += ".html";
// 			std::cout << name << std::endl;
// 			int fd = open(name.c_str(), O_TRUNC|O_CREAT|O_RDWR, 0664);
// 			if (fd < 0)
// 			{
// 				std::cerr << "FAILED\n";
// 			}
// 			int pid = fork();
// 			if (!pid)
// 			{
// 				dup2(fd, 1);
// 				close(fd);
// 				execve(line.c_str(), args, ee.data());
// 				exit(0);
// 			}
// 			wait(NULL);
// 			bodyPath = name;
// 			break;
// 		}
// 	}
// }

void Response::prepareGET()
{
    if (isWorking)
        return;
    std::string resource = locationCTX.getRoot() + Utils::urlDecoding(request->getRequestPath());
    if (!Utils::checkIfPathExists(resource))
        throw ResponseErrorException(*this, NotFound);
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
                        throw ResponseErrorException(*this, FORBIDDEN);
                    return;
                }
				// cgi
                bodyPath = resource + index;
                statusCode = OK;
            }
            catch (const Utils::FilePermissionDenied& e)
            {
                throw ResponseErrorException(*this, FORBIDDEN);
            }
            catch (const Utils::FileNotFoundException& e)
            {
                throw ResponseErrorException(*this, NotFound);
            }
        }
    }
    else
    {
        if (!Utils::isReadableFile(resource))
            throw ResponseErrorException(*this, FORBIDDEN);
		// cgi
        bodyPath = resource;
        std::string rangeHeader = request->getHeaderByName("Range");
        if (!rangeHeader.empty())
        {
            size_t i = rangeHeader.find('=');
            if (i != std::string::npos)
            {
                std::string unit = rangeHeader.substr(0, i);
                if (unit == "bytes" || unit == "Bytes")
                {
                    isPartialContent = true;
                    std::string range = rangeHeader.substr(i + 1, rangeHeader.length());
                    i = range.find('-');
                    if (i != std::string::npos)
                    {
                        startOffset = std::atoll(range.substr(0, i).c_str());
                        endOffset = std::atoll(range.substr(i + 1, range.length()).c_str());
                        if (endOffset == 0)
                            endOffset = Utils::getFileSize(bodyPath);
                        endOffset--;
                    }
                    statusCode = PartialContent;
                    isWorking = true;
                    return;
                }
            }
            std::cout << "Start_Offset = " << startOffset << "  " << "End_Offset = " << endOffset << std::endl;
        }
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
    std::string html = Utils::replaceAll(AUTO_INDEX_TEMPLATE, "$indexof$", request->getRequestPath());
    dir = opendir(path.c_str());
    if (!dir)
    {
        throw ResponseErrorException(*this, FORBIDDEN);
    }
    while ((entry = readdir(dir)))
    {
        if (Utils::isDirectory(path + entry->d_name))
            html += "<tr><td><i class='glyphicon glyphicon-folder-close'></i>";
        else
            html += "<tr><td><i class='glyphicon glyphicon-file'></i>";
        html+= "<a style='text-decoration:none'; href='" + Utils::urlEncoding(std::string(entry->d_name)) + "'> " + entry->d_name + "</td>";
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

void Response::preparePOST()
{
    if (!locationCTX.getUploadStore().empty())
    {
        statusCode = Created;
        body = generateHtmlErrorPage();
        bodyPath.clear();
    }
    else
    {
        std::string resource = locationCTX.getRoot() + Utils::urlDecoding(request->getRequestPath());
        if (!Utils::checkIfPathExists(resource))
            throw ResponseErrorException(*this, NotFound);
        if (Utils::isDirectory(resource))
        {
            if (!Utils::stringEndsWith(resource, "/"))
                prepareRedirection(MovedPermanently, request->getRequestPath() + "/");
            else
            {
                try
                {
                    std::string index = locationCTX.getIndex(resource);
                    if (index.empty() || !hasCGI)
                        throw Utils::FilePermissionDenied();
                    /* Request BODY goes to CGI !! */
                }
                catch (const std::exception& e)
                {
                    throw ResponseErrorException(*this, FORBIDDEN);
                }
            }
        }
        else
        {
            if (!hasCGI)
                throw ResponseErrorException(*this, FORBIDDEN);
        }
    }
}

// void Response::copyEnv()
// {
// 	std::string	req_path = request->getRequestPath();
// 	env.push_back("SERVER_PROTOCOL=");
// 	env.back() += request->getProtocol();
// 	env.push_back("SERVER_PORT=");
// 	env.back() += serverCTX.getPort();
// 	env.push_back("REQUEST_METHOD=");
// 	env.back() += request->getMethod();
// 	env.push_back("PATH_INFO=");
// 	env.back() += req_path;
// 	env.push_back("PATH_TRANSLATED=");
// 	env.back() += locationCTX.getRoot() + req_path;
// 	env.push_back("SCRIPT_NAME=");
// 	env.back() += bodyPath.substr(bodyPath.find_last_of('/') + 1);
// 	env.push_back("DOCUMENT_ROOT=");
// 	env.back() += bodyPath.substr(0, bodyPath.find_last_of('/'));
// 	env.push_back("QUERY_STRING=");
// 	env.back() += request->getQueryString();
// 	env.push_back("CONTENT_TYPE=");
// 	env.back() += request->getHeaderByName("CONTENT-TYPE");
// 	env.push_back("CONTENT_LENGTH=");
// 	env.back() += request->getHeaderByName("CONTENT-LENGTH");
// 	env.push_back("PATH=");
// 	env.back() += PATH;
// 	env.push_back("REDIRECT_STATUS=");
// 	env.back() += Utils::intToString(request->getStatus());
// 	Map m = request->getHeaders();
// 	for (Map::iterator it = m.begin(); it != m.end(); it++)
// 	{
// 		std::string first = it->first;
// 		Utils::toLower(first);
// 		if (first != "content-length" && first != "content-type")
// 			env.push_back(Utils::envName(it->first) + "=" + it->second);
// 	}
// }

void Response::prepareResponse()
{
    try
    {
        if (request->getStatus() >= 400)
            throw ResponseErrorException(*this, request->getStatus());
        if (locationCTX.getHttpRedirection().size() > 0)
        {
            prepareRedirection(Utils::stringToInt(locationCTX.getHttpRedirection()[0]), locationCTX.getHttpRedirection()[1]);
            prepareBody();
            prepareHeaders();
            responseDone = true;
            return;
        }
        if (request->getMethod() == "GET")
        {
            prepareGET();
			// if (fd == INT_MIN)
			// {
			// 	copyEnv();
			// 	prepareCGI();
			// }
            prepareBody();
            prepareHeaders();
        }
        else if (request->getMethod() == "POST")
        {
            preparePOST();
            prepareBody();
            prepareHeaders();
        }
        else if (request->getMethod() == "DELETE")
        {
            /* Prepare DELETE */
        }
        else
        {
            
        }
    }
    catch (const ResponseErrorException& e) { }
}

void Response::resetResponse()
{
    fd = INT_MIN;
    request = NULL;
    statusCode = 200;
    isWorking = false;
    headersSent = false;
    responseDone = false;
    isRedirection = false;
	env.clear();
    body.clear();
    headers.clear();
    bodyPath.clear();
    location.clear();
    hasCGI = false;
    isPartialContent = false;
    startOffset = 0;
    endOffset = 0;
    alreadySeeked = false;
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

// void Response::setupEnv(char **_env)
// {
// 	std::string	var;
// 	env_ptr = _env;
// 	for (size_t i = 0; env_ptr && env_ptr[i]; i++)
// 	{
// 		var = env_ptr[i];
// 		if (Utils::stringStartsWith(var, "PATH="))
// 		{
// 			PATH =  var.substr(5);
// 			break;
// 		}
// 	}
// 	if (!env_ptr || Response::PATH.empty())
// 		throw Fail("needs PATH variable");
// }

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