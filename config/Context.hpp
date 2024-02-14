/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/14 15:24:28 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include <array>
#include <string>
#include <fstream>

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

        /*Useful getters for request*/
        const DirectivesMap& getDirectives() const;
        std::string getRoot() const;
        std::string getIndex() const;
        bool getAutoIndex() const;
        int getClientMaxBodySize() const;
        std::array<std::string, 3> getAllowedMethods() const;
        std::string getUploadStore() const;
};