/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:31:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/09 14:02:27 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationContext.hpp"

LocationContext::LocationContext()
{

}

LocationContext::LocationContext(const std::string& prefix)
{
    this->prefix = prefix;
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

const std::string &LocationContext::getPrefix() const
{
    return prefix;
}
