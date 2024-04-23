/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:40:11 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/23 20:52:37 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Utils.hpp"
#include "Context.hpp"
#include "LocationContext.hpp"

class ServerContext : public Context
{
    private:
        LocationsVector locations;
    public:
        ServerContext();
        ServerContext(const ServerContext& obj);
        ServerContext& operator=(const ServerContext& obj);
        ~ServerContext();

        void addLocation(LocationContext location);
        LocationsVector& getLocations();
        LocationContext matchLocation(const std::string& prefix);
        std::string getListen();
        std::string getHost();
        std::string getPort();
        StringVector getServerName();
};