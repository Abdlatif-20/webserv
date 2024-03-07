/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:24 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/07 14:01:29 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "ServerContext.hpp"

class Response
{
    private:
        ServerContext serverCtx;
        Request request;

        int statusCode;
        std::map<std::string, std::string> headers;
        std::string body;

        std::string toString();
        std::string generateHtmlErrorPage();
        static std::map<int, std::string> reasonPhrases;
        static std::map<std::string, std::string> mimeTypes;
    public:
        Response();
        Response(const Response& obj);
        Response& operator=(const Response& obj);
        ~Response();
        
        /* Setters */
        void setServerCtx(const ServerContext& serverCtx);
        void setRequest(const Request& request);
        void setStatusCode(int statusCode);
        void addHeader(const std::string& key, const std::string& value);
        void setBody(const std::string& body);

        /* Getters */
        const ServerContext& getServerCtx() const;
        const Request& getRequest() const;
        int getStatusCode() const;
        const std::map<std::string, std::string>& getHeaders() const;
        const std::string& getBody() const;
        std::string getMimeType(const std::string& extension);

        std::string generateResponse();

        static void initReasonPhrases();
        static void initMimeTypes();
};