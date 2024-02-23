/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:09:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/20 13:03:33 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include <vector>

class Core
{
    private:
        std::vector<Server> servers;
    public:
        Core();
        Core(const Core& obj);
        Core& operator=(const Core& obj);
        ~Core();
};