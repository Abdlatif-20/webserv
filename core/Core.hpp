/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:09:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/29 20:21:51 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include <poll.h>

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
};