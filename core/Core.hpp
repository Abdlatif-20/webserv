/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:09:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/04 05:59:13 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <poll.h>

#define BUFFER_SIZE 1024
class Core
{
    private:
        Config config;
        std::vector<Server> servers;
        std::vector<Client> clients;
        std::vector<pollfd> poll_fds;
    public:
        Core();
        Core(const Config& config);
        Core(const Core& obj);
        Core& operator=(const Core& obj);
        ~Core();

        void startWorking();
        bool    ifShortRequest(char *buffer, std::string &temp, std::vector<Client> &clients, int i);
};