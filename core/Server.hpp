/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 21:23:57 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/24 18:34:00 by mel-yous         ###   ########.fr       */
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
#include <vector>
#include <poll.h>

class Server
{
    private:
        std::string host;
        std::string port;
        addrinfo *serverInfo; //to be added to assigment operator
        int server_fd;
    public:
        Server();
        Server(const std::string& host, const std::string& port);
        Server(const Server& obj);
        Server& operator=(const Server& obj);
        ~Server();

        const std::string& getServerHost();
        const std::string& getServerPort();
        addrinfo* getServerInfo();
        int getServer_fd();

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