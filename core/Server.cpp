/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 21:23:59 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/22 17:44:24 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>

Server::Server()
{

}

Server::Server(const std::string& host, const std::string& port)
{
    addrinfo hints, *result;
    this->host = host;
    this->port = port;
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (server_fd == -1)
        throw ServerErrorException(strerror(errno));
    fcntl(server_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_IP;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &result) == -1)
        throw ServerErrorException(strerror(errno));
    this->serverInfo = result;
    if (bind(server_fd, result->ai_addr, result->ai_addrlen) == -1)
        throw ServerErrorException(strerror(errno));
    if (listen(server_fd, 32) == -1)
        throw ServerErrorException(strerror(errno));
}


Server::Server(const Server& obj)
{
    *this = obj;
}

Server& Server::operator=(const Server& obj)
{
    if (this == &obj)
        return *this;
    host = obj.host;
    port = obj.port;
    serverInfo = obj.serverInfo;
    server_fd = obj.server_fd;
    return *this;
}

Server::~Server()
{
    // freeaddrinfo(serverInfo);
}

const std::string& Server::getServerHost()
{
    return host;
}

const std::string& Server::getServerPort()
{
    return port;
}

addrinfo* Server::getServerInfo()
{
    return serverInfo;
}

int Server::getServer_fd()
{
    return server_fd;
}

Server::ServerErrorException::ServerErrorException(const std::string& str)
{
    this->str = "Server error, " + str;
}

const char* Server::ServerErrorException::what() const throw()
{
    return str.c_str();
}

Server::ServerErrorException::~ServerErrorException() throw()
{

}