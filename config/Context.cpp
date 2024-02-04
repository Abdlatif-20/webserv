/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/04 15:37:37 by mel-yous         ###   ########.fr       */
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

const DirectivesMap& Context::getDirectives() const
{
    return directives;
}