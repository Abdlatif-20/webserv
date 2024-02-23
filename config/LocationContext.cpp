/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:31:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/18 13:44:07 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationContext.hpp"

LocationContext::LocationContext()
{

}

/* The `LocationContext::LocationContext(const std::string& prefix)` constructor of the
`LocationContext` class is a parameterized constructor that initializes a `LocationContext` object
with a specified `prefix` value. It sets the `prefix` member variable of the `LocationContext`
object to the value passed as an argument to the constructor. */
LocationContext::LocationContext(const std::string& prefix)
{
    this->prefix = prefix;
}

LocationContext::LocationContext(const LocationContext& obj) : Context(obj)
{
    prefix = obj.prefix;
}

LocationContext& LocationContext::operator=(const LocationContext& obj)
{
    if (this == &obj)
        return *this;
    Context::operator=(obj);
    prefix = obj.prefix;
    return *this;
}

LocationContext::~LocationContext()
{

}

/* The `const std::string &LocationContext::getPrefix() const` function is a member function of the
`LocationContext` class. It returns the value of the prefix. */
const std::string &LocationContext::getPrefix() const
{
    return prefix;
}
