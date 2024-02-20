/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 21:23:57 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/19 18:34:57 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <exception>
#include <limits>
#include <errno.h>
#include <iostream>

class Server
{
    private:
        std::string host;
        int sockFD;
        int rw_sockFD;
        addrinfo* sockAddrInfo;
    public:
        Server();
        Server(const std::string& host);
        Server(const Server& obj);
        Server& operator=(const Server& obj);
        ~Server();

        void Start();
        void Accept();
        ssize_t Receive(char* buffer, size_t len = 1023);
        void Send(char* buffer);
        
        class ServerErrorException : public std::exception
        {
            private:
                std::string str;
            public:
                ServerErrorException(const std::string& str);
                const char* what() const throw();
                ~ServerErrorException() throw();
        };
};