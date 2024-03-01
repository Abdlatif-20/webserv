/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:40:09 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/29 11:01:47 by mel-yous         ###   ########.fr       */
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
const LocationsVector& ServerContext::getLocations() const
{
    return locations;
}

const LocationContext ServerContext::matchLocation(const std::string& prefix) const
{
    LocationsVector::const_iterator it = locations.cbegin();
    while (it != locations.cend())
    {
        if (it->getPrefix() == prefix)
            return *it;
        it++;
    }
    it = locations.cbegin();
    LocationContext result;
    while (it != locations.cend())
    {
        if (Utils::stringStartsWith(prefix, it->getPrefix())
            && it->getPrefix().length() > result.getPrefix().length())
            result = *it;
        it++;
    }
    return result;
}

std::string ServerContext::getListen() const
{
    DirectivesMap::const_iterator it = directives.find("listen");
    if (it != directives.cend())
        return *it->second.cbegin();
    return "8080";
}

std::string ServerContext::getHost() const
{
    std::string str = getListen();
    size_t i = str.find(':');
    if (i != std::string::npos)
        return str.substr(0, i);
    return "0.0.0.0";
}

std::string ServerContext::getPort() const
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
StringVector ServerContext::getServerName() const
{
    DirectivesMap::const_iterator it = directives.find("server_name");
    if (it != directives.cend())
        return it->second;
    StringVector vec;
    vec.push_back("localhost");
    return vec;
}