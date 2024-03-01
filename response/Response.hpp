/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:24 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/29 15:46:07 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Defs.hpp"
#include <iostream>
#include <map>
#include "Request.hpp"
#include "Config.hpp"

class Response
{
    private:
        ServerContext serverCtx;
        Request req;
        int status;
        std::string body;
    public:
        Response();
        Response(const Request& req, const ServerContext& serverCtx);
        Response(const Response& obj);
        Response& operator=(const Response& obj);
        ~Response();

        const std::string generateResponse();
};