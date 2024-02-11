/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/11 18:41:39 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Context.hpp"

Context::Context()
{

}

Context::Context(const Context& obj)
{
    *this = obj;
}

Context& Context::operator=(const Context& obj)
{
    if (this == &obj)
        return *this;
    directives = obj.directives;
    return *this;
}

Context::~Context()
{

}

void Context::addDirective(Directive _directive)
{
    directives.insert(_directive);
}

void Context::appendDirective(Directive _directive)
{
    DirectivesMap::iterator it = directives.find(_directive.first);
    it->second.insert(it->second.end(), _directive.second.begin(), _directive.second.end());
}

const DirectivesMap& Context::getDirectives() const
{
    return directives;
}

StringVector::const_iterator Context::getDirectiveByKey(const std::string& key) const
{
    DirectivesMap::const_iterator it = directives.find(key);
    if (it != directives.end())
        return it->second.cbegin();
    return it->second.cend();
}