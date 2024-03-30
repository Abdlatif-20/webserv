/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:24 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/30 18:34:36 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CGI.hpp"
#include "Request.hpp"
#include "ServerContext.hpp"
#include "ResponseUtils.hpp"
#include <dirent.h>
#include <map>

class Response
{
    private:
        Request *request;
        ServerContext serverCTX;
        LocationContext locationCTX;

        char buffer[1024];
        std::ifstream ifs;

        /*Attributes for handling Ranged Request*/
        bool isRanged;
        ssize_t startOffset;
        ssize_t endOffset;
        bool alreadySeeked;
        ssize_t sendedBytes;
        size_t readSize;

        int statusCode;
        std::string statusLine;
        std::map<std::string, std::string> headers;
        std::string body;
        std::string bodyPath;
        bool headersSent;
        bool responseDone;
        bool isWorking;
        bool isRedirection;
        std::string location;

        std::vector<std::string> env;

        std::string generateHtmlErrorPage();
        bool checkErrorPage(const std::string& path);

        void generateResponseError();
        void prepareHeaders();
        void prepareBody();
        void prepareGET();
        void preparePOST();
        void prepareDELETE();
        void prepareRedirection(int _status, const std::string& _location);
        void autoIndex(const std::string& path);

        void runCGI();
        void handleRange();
        void prepareRanged();

        static std::map<int, std::string> reasonPhrases;
        static std::map<std::string, std::string> mimeTypes;

    public:

        Response();
        Response(const Response& obj);
        Response& operator=(const Response& obj);
        ~Response();
        
        void setRequest(Request* request);
        void setServerCTX(const ServerContext& serverCTX);
        void setLocationCTX(const LocationContext& locationCTX);
        void setHeadersSent(bool flag);
        static std::string getMimeType(const std::string& extension);
        const std::string& getBody() const;
        const std::string& getHeaderByName(const std::string& name);
        bool getHeadersSent() const;
        bool responseIsDone() const;

        void resetResponse();
        void prepareResponse();

        std::map<std::string, std::string>& getHeaders();
        std::string headersToString();

		ServerContext&	getServerCtx();
		LocationContext&	getLocationCtx();

        static void initMimeTypes();
        static void initReasonPhrases();
};