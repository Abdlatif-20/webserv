/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:09:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/04 22:15:59 by aben-nei         ###   ########.fr       */
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
    char buffer[BUFFER_SIZE];
    std::string temp ="";
    std::vector<pollfd>::iterator pollfd_it;

    while (true)
    {
        it = servers.begin();
        while (it != servers.end())
        {
            
            client_fd = accept(it->getServer_fd(), it->getServerInfo()->ai_addr, &it->getServerInfo()->ai_addrlen);
            if (client_fd != -1)
            {
                std::cout << "Client: " << client_fd << " accepted" << std::endl;
                clients.push_back(Client(client_fd));
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
                clients[i].getRequest().parseRequest(std::string(buffer, clients[i].getRecvBytes()), config);
                // std::cout<<"status of client [" << clients[i].getClient_fd() << "] : " << clients[i].getRequest().getStatus() << std::endl;
            }
            if ((poll_fds[i].revents & POLLOUT) && clients[i].getRequest()._requestIsDone)
            {
                std::cout << "Client [" << clients[i].getClient_fd() << "] REQUEST DONE" << std::endl;
                clients[i].getRequest()._requestIsDone = false;
            }
            if (poll_fds[i].fd > 0 && (poll_fds[i].revents & POLLHUP))
                close(clients[i].getClient_fd());
        }
    }
}