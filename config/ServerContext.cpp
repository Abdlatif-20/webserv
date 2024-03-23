/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:40:09 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/23 20:54:12 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerContext.hpp"

ServerContext::ServerContext()
{

}

ServerContext::ServerContext(const ServerContext& obj) : Context(obj)
{
    locations = obj.locations;
}

ServerContext& ServerContext::operator=(const ServerContext& obj)
{
    if (this == &obj)
        return *this;
    Context::operator=(obj);
    locations = obj.locations;
    return *this;
}

ServerContext::~ServerContext()
{

}

/* The `void ServerContext::addLocation(LocationContext location)` function in the `ServerContext`
class is used to add a `LocationContext` object to the `locations` vector within the `ServerContext` object.*/
void ServerContext::addLocation(LocationContext location)
{
    this->locations.push_back(location);
}

/* The `const LocationsVector& ServerContext::getLocations() const` function in the `ServerContext`
class is a getter method that returns a constant reference to the `locations` vector within the `ServerContext` object.*/
LocationsVector& ServerContext::getLocations()
{
    return locations;
}

LocationContext ServerContext::matchLocation(const std::string& prefix)
{
    LocationsVector::iterator it = locations.begin();
    while (it != locations.end())
    {
        if (it->getPrefix() == prefix)
            return *it;
        it++;
    }
    it = locations.begin();
    LocationContext result;
    while (it != locations.end())
    {
        if (Utils::stringStartsWith(prefix, it->getPrefix())
            && it->getPrefix().length() > result.getPrefix().length())
            result = *it;
        it++;
    }
    return result;
}

std::string ServerContext::getListen()
{
    DirectivesMap::iterator it = directives.find("listen");
    if (it != directives.end())
        return *it->second.begin();
    return "8080";
}

std::string ServerContext::getHost()
{
    std::string str = getListen();
    size_t i = str.find(':');
    if (i != std::string::npos)
        return str.substr(0, i);
    return "0.0.0.0";
}

std::string ServerContext::getPort()
{
    std::string str = getListen();
    size_t i = str.find(':');
    if (i != std::string::npos)
        return str.substr(i + 1, str.length() - i);
    return str;
}

/* The `StringVector ServerContext::getServerName() const` function in the `ServerContext` class is a
getter method that retrieves the value associated with the "server_name" directive from the
`directives` map within the `ServerContext` object. */
StringVector ServerContext::getServerName()
{
    DirectivesMap::iterator it = directives.find("server_name");
    if (it != directives.end())
        return it->second;
    StringVector vec;
    vec.push_back("localhost");
    return vec;
}