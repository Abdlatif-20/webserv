/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:43 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/26 16:58:26 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "Response.hpp"

class Client
{
    private:
        int client_fd;
        ssize_t recvBytes;
        Request req;
        Response resp;
    public:
        Client();
        Client(int client_fd);
        Client(const Client& obj);
        Client& operator=(const Client& obj);
        ~Client();

        int getClient_fd() const;
        ssize_t getRecvBytes() const;
        void setRecvBytes(ssize_t r);
        Request& getRequest();
};