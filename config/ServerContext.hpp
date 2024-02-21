/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:40:11 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/21 11:10:45 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
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
        const LocationsVector& getLocations() const;
        std::string getListen() const;
        std::string getHost() const;
        std::string getPort() const;
        StringVector getServerName() const;
};