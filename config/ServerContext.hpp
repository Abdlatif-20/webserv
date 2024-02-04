/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:40:11 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/04 16:00:05 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Context.hpp"
#include "LocationContext.hpp"

class ServerContext : public Context
{
    private:
        std::vector<LocationContext> locations;
    public:
        ServerContext();
        ServerContext(const ServerContext& obj);
        ServerContext& operator=(const ServerContext& obj);
        ~ServerContext();

        void addLocation(LocationContext location);
};