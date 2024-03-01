/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 14:07:22 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/01 11:00:49 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{

}

Response::Response(const Request& req, const ServerContext& serverCtx)
{
    this->req = req;
    this->serverCtx = serverCtx;
    this->body = "";
    status = req.getStatus();
}

Response::Response(const Response &obj)
{
    *this = obj;
}

Response& Response::operator=(const Response& obj)
{
    if (this == &obj)
        return *this;
    this->serverCtx = obj.serverCtx;
    this->req = obj.req;
    this->status = obj.status;
    this->body = obj.body;
    return *this;
}

Response::~Response()
{

}

const std::string Response::generateResponse()
{
    std::string responseText;
    return responseText;
}