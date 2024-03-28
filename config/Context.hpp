/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/27 20:05:26 by mel-yous         ###   ########.fr       */
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
        std::map<std::string, std::string> cgi;
    public:
        Context();
        Context(const Context& obj);
        Context& operator=(const Context& obj);
        virtual ~Context();

        void addDirective(Directive _directive);
        void appendDirective(Directive _directive);
        void addErrorPage(StringVector error_page);
        void addCGI(std::pair<std::string, std::string>_pair);

        /*Useful getters for request*/
        DirectivesMap& getDirectives();
        std::vector<StringVector>& getErrorPages();
        std::string getRoot();
        std::string getIndex(const std::string& path);
        bool getAutoIndex();
        int getClientMaxBodySize();
        StringVector getAllowedMethods();
        std::string getUploadStore();
        std::string getErrorPage(const std::string& status);
        StringVector getHttpRedirection();
        std::map<std::string, std::string> getCGI();
        unsigned int getCGI_timeout();
};