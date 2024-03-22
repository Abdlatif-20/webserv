/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:24 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/21 15:48:34 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "ServerContext.hpp"
#include <dirent.h>

// class Request;

class Response
{
    private:
        Request *request;
        Context *context;

        char buffer[1024];
        int fd;

        int statusCode;
        std::string headers;
        std::string body;
        std::string bodyPath;
        bool headersSent;
        bool responseDone;
        bool isWorking;
        bool isRedirection;
        std::string location;
        bool hasCGI;

        std::string generateHtmlErrorPage();
        bool checkErrorPage(const std::string& path);

        void generateResponseError();
        void prepareHeaders();
        void prepareBody();
        void prepareGET();
        void prepareRedirection(int _status, const std::string& _location);
        void autoIndex(const std::string& path);
        
        void preparePOST();

        static std::map<int, std::string> reasonPhrases;
        static std::map<std::string, std::string> mimeTypes;
    public:
        Response();
        Response(const Response& obj);
        Response& operator=(const Response& obj);
        ~Response();
        
        void setRequest(Request* request);
        void setContext(Context* context);
        void setHeadersSent(bool flag);
        static std::string getMimeType(const std::string& extension);
        const std::string& getBody() const;
        const std::string& getHeaders() const;
        bool getHeadersSent() const;
        bool responseIsDone() const;

        void prepareResponse();
        void resetResponse();

        static void initReasonPhrases();
        static void initMimeTypes();

        class ResponseErrorException : public std::exception
        {
            public:
                int status;
                ResponseErrorException() { };
                ResponseErrorException(Response& response, int _status)
                {
                    response.statusCode = _status;
                    response.generateResponseError();
                    response.prepareBody();
                    response.prepareHeaders();
                }
                ~ResponseErrorException() throw() { };
        };
};