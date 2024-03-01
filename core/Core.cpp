/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:09:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/29 19:45:02 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core.hpp"

#define fakeResponse "HTTP/1.1 404 Not Found\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 4\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\nabcd"

Core::Core()
{

}

Core::Core(const Config& _config)
{
    this->config = _config;
    ServersVector::const_iterator serv_it = _config.getServers().cbegin();
    while (serv_it != _config.getServers().cend())
    {
        Server virtualServer(serv_it->getHost(), serv_it->getPort());
        this->servers.push_back(virtualServer);
        serv_it++;
    }
}

Core::Core(const Core& obj)
{
    *this = obj;
}

Core& Core::operator=(const Core& obj)
{
    if (this == &obj)
        return *this;
    config = obj.config;
    servers = obj.servers;
    clients = obj.clients;
    poll_fds = obj.poll_fds;
    return *this;
}

Core::~Core()
{

}

void Core::startWorking()
{
    std::vector<Server>::iterator it;
    int client_fd;
    char buffer[1024];
    std::vector<pollfd>::iterator pollfd_it;

    while (true)
    {
        it = servers.begin();
        while (it != servers.end())
        {
            client_fd = accept(it->getServer_fd(), it->getServerInfo()->ai_addr, &it->getServerInfo()->ai_addrlen);
            if (client_fd != -1)
            {
                Client newClient(client_fd);
                clients.push_back(newClient);
                poll_fds.push_back((pollfd){client_fd, POLLIN | POLLOUT, 0});
            }
            it++;
        }
        poll(poll_fds.data(), poll_fds.size(), 0);
        for (size_t i = 0; i < poll_fds.size(); i++)
        {
            if (poll_fds[i].revents & POLLIN)
            {
                bzero(buffer, sizeof(buffer));
                clients[i].setRecvBytes(recv(poll_fds[i].fd, buffer, 1023, 0));
                clients[i].getRequest().parseRequest(buffer, config);
            }
            if ((poll_fds[i].revents & POLLOUT) && clients[i].getRequest()._requestIsDone)
            {
                Response resp(clients[i].getRequest(), *config.getServers().cbegin());
                std::string responseText = resp.generateResponse();
                std::cout << responseText.c_str() << std::endl;
                send(poll_fds[i].fd, responseText.c_str(), responseText.length(), 0);
                clients[i].getRequest()._requestIsDone = false;
                // send(poll_fds[i].fd, fakeResponse, 209, 0);
                // clients[i].getRequest()._requestIsDone = false;
                // close(poll_fds[i].fd);
                // if ((poll_fds.begin() + i) != poll_fds.end())
                //     poll_fds.erase(poll_fds.cbegin() + i);
            }
            if (poll_fds[i].fd > 0 && (poll_fds[i].revents & POLLHUP))
            {
                close(clients[i].getClient_fd());
                if ((poll_fds.begin() + i) != poll_fds.end())
                    poll_fds.erase(poll_fds.cbegin() + i);
            }
        }
    }
}