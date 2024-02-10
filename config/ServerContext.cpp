/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:40:09 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/04 16:55:27 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerContext.hpp"

ServerContext::ServerContext()
{

}

ServerContext::ServerContext(const ServerContext& obj)
{
    *this = obj;
}

ServerContext& ServerContext::operator=(const ServerContext& obj)
{
    if (this == &obj)
        return *this;
    directives = obj.directives;
    locations = obj.locations;
    return *this;
}

ServerContext::~ServerContext()
{

}

void ServerContext::addLocation(LocationContext location)
{
    this->locations.push_back(location);
}

const LocationsVector& ServerContext::getLocations() const
{
    return locations;
}