/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:24 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/10 19:08:56 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "ServerContext.hpp"

class Response
{
    enum state
    {
        Initial,
        Reading,
        Finished
    };

    private:
        Request *request;
        Context *context;
        
        int statusCode;
        
        std::string generateHtmlErrorPage();
        static std::map<int, std::string> reasonPhrases;
        static std::map<std::string, std::string> mimeTypes;
    public:
        Response();
        Response(const Response& obj);
        Response& operator=(const Response& obj);
        ~Response();
        
        /* Setters */
        void setRequest(Request* request);

        std::string getMimeType(const std::string& extension);

        static void initReasonPhrases();
        static void initMimeTypes();
};