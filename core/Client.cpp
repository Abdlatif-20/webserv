/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:41 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/22 17:59:38 by mel-yous         ###   ########.fr       */
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