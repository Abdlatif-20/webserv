/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/26 05:59:29 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{

}

Client::Client(int client_fd)
{
    this->client_fd = client_fd;
    this->recvBytes = -1;
}

Client::Client(const Client& obj)
{
    *this = obj;
}

Client& Client::operator=(const Client& obj)
{
    if (this == &obj)
        return *this;
    client_fd = obj.client_fd;
    recvBytes = obj.recvBytes;
    req = obj.req;
    return *this;
}

Client::~Client()
{

}

int Client::getClient_fd() const
{
    return client_fd;
}

ssize_t Client::getRecvBytes() const
{
    return recvBytes;
}

void Client::setRecvBytes(ssize_t r)
{
    recvBytes = r;
}

Request& Client::getRequest()
{
    return req;
}

Response& Client::getResponse()
{
    return res;
}