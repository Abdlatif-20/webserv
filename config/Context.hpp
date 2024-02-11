/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/11 16:58:55 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include <string>

class Context
{
    protected:
        DirectivesMap directives;
    public:
        Context();
        Context(const Context& obj);
        Context& operator=(const Context& obj);
        ~Context();

        void addDirective(Directive _directive);
        void appendDirective(Directive _directive);
        const DirectivesMap& getDirectives() const;
        StringVector::const_iterator getDirectiveByKey(const std::string& key) const;
};