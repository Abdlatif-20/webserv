/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:40:09 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/15 13:23:44 by mel-yous         ###   ########.fr       */
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

std::string ServerContext::getListen() const
{
    DirectivesMap::const_iterator it = directives.find("listen");
    if (it != directives.cend())
        return *it->second.cbegin();
    return "8080";
}

StringVector ServerContext::getServerName() const
{
    DirectivesMap::const_iterator it = directives.find("server_name");
    if (it != directives.cend())
        return it->second;
    StringVector vec;
    vec.push_back("localhost");
    return vec;
}