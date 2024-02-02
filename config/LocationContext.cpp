/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:31:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/02 20:27:19 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationContext.hpp"

LocationContext::LocationContext()
{

}

LocationContext::LocationContext(const LocationContext& obj)
{
    *this = obj;
}

LocationContext& LocationContext::operator=(const LocationContext& obj)
{
    if (this == &obj)
        return *this;
    prefix = obj.prefix;
    directives = obj.directives;
    return *this;
}

LocationContext::~LocationContext()
{

}

void LocationContext::setPrefix(const std::string& prefix)
{
    this->prefix = prefix;
}

void LocationContext::addDirective(Directive _directive)
{
    directives.insert(_directive);
}