/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:24 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/19 22:36:32 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "ServerContext.hpp"
#include <dirent.h>

class Response
{
    enum Status
    {
        FORBIDDEN = 403  
    };
    private:
        Request *request;
        Context *context;

        char buffer[1024];// modified
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

        std::string generateHtmlErrorPage();
        bool checkErrorPage(const std::string& path);
        void generateResponseError();
        void prepareHeaders();
        void prepareGETBody();
        void prepareCGI();
        void prepareGET();
        void listDirectories(const std::string& path);

        static std::map<int, std::string> reasonPhrases;
        static std::map<std::string, std::string> mimeTypes;

    public:
		static char	**env;
		static std::string	PATH;

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


        static void initMimeTypes();
        static void initReasonPhrases();
		static void	setupEnv(char **_env);
};