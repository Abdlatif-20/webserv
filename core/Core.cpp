/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:09:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/23 09:16:07 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core.hpp"

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
    servers = obj.servers;
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
    std::vector<pollfd> poll_fds;
    std::vector<pollfd>::iterator pollfd_it;

    while (true)
    {
        it = servers.begin();
        while (it != servers.end())
        {
            // std::cout << it->getPoll_fd()->fd << std::endl;
            client_fd = accept(it->getServer_fd(), it->getServerInfo()->ai_addr, &it->getServerInfo()->ai_addrlen);
            if (client_fd != -1)
            {
                std::cout << "Client: " << client_fd << " accepted" << std::endl;
                Client newClient(client_fd);
                clients.push_back(newClient);
                pollfd p;
                p.fd = client_fd;
                p.events = POLL_IN;
                p.revents = 0;
                poll_fds.push_back(p);
            }
            it++;
        }
        poll(poll_fds.data(), poll_fds.size(), 100);
        for (size_t i = 0; i < poll_fds.size(); i++)
        {
            if (poll_fds[i].revents == POLLIN)
            {
                bzero(buffer, sizeof(buffer));
                clients[i].setRecvBytes(recv(poll_fds[i].fd, buffer, 1023, 0));
                std::cout << buffer;
            }
        }
    }
}