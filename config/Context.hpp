/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/08 16:24:38 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include "Utils.hpp"
#include <array>
#include <string>
#include <fstream>
#include <iostream>

class Context
{
    protected:
        DirectivesMap directives;
        std::vector<StringVector> errorPages;
    public:
        Context();
        Context(const Context& obj);
        Context& operator=(const Context& obj);
        virtual ~Context();

        void addDirective(Directive _directive);
        void appendDirective(Directive _directive);
        void addErrorPage(StringVector error_page);

        /*Useful getters for request*/
        const DirectivesMap& getDirectives() const;
        const std::vector<StringVector>& getErrorPages() const;
        std::string getRoot() const;
        std::string getIndex() const;
        bool getAutoIndex() const;
        int getClientMaxBodySize() const;
        StringVector getAllowedMethods() const;
        std::string getUploadStore() const;
        std::string getErrorPage(const std::string& status) const;
};