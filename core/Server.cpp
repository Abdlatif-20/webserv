/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 21:23:59 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/19 18:35:55 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{

}

Server::Server(const std::string& host)
{
    addrinfo hints;
    addrinfo *result = NULL;
    size_t i = host.find(":");
    std::string ipAddress = "0.0.0.0"; /*Default value of used IP is `ANY IPADDRESS`*/
    std::string port = host; /*Default value of port is `listen` directive*/
    std::memset(&hints, 0, sizeof(hints));
    this->host = host;
    this->sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (this->sockFD == -1)
        throw ServerErrorException(strerror(errno));
    if (i != std::string::npos)
    {
        ipAddress = host.substr(0, i);
        port = host.substr(i + 1, host.length() - i);
    }
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_IP;
    if (getaddrinfo(ipAddress.c_str(), port.c_str(), &hints, &result) == -1)
        throw ServerErrorException(strerror(errno));
    sockAddrInfo = result;
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
    sockFD = obj.sockFD;
    return *this;
}

Server::~Server()
{
    freeaddrinfo(sockAddrInfo);
    close(sockFD);
}

void Server::Start()
{
    if (bind(sockFD, sockAddrInfo->ai_addr, sockAddrInfo->ai_addrlen) == -1)
        throw ServerErrorException(strerror(errno));
    listen(sockFD, 128);
}

void Server::Accept()
{
    rw_sockFD = accept(sockFD, sockAddrInfo->ai_addr, &sockAddrInfo->ai_addrlen);
    if (rw_sockFD == -1)
        throw ServerErrorException(strerror(errno));
    std::cout << "Client connected" << std::endl;
}

ssize_t Server::Receive(char* buffer, size_t len)
{
    ssize_t sz = recv(rw_sockFD, buffer, len, 0);
    if (sz == -1)
        throw ServerErrorException(strerror(errno));
    return sz;
}

void Server::Send(char* buffer)
{
    if (send(rw_sockFD, buffer, 1024, 0) == -1)
        throw ServerErrorException(strerror(errno));
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