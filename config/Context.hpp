/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/04/16 12:15:19 by mel-yous         ###   ########.fr       */
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
        std::map<std::string, std::string> errorPages;
        std::map<std::string, std::string> cgi;
    public:
        Context();
        Context(const Context& obj);
        Context& operator=(const Context& obj);
        virtual ~Context();

        void addDirective(Directive _directive);
        void appendDirective(Directive _directive);
        void addErrorPage(std::string key, std::string value);
        void addCGI(std::string key, std::string value);

        /*Useful getters for request*/
        DirectivesMap& getDirectives();
        std::map<std::string, std::string>& getErrorPages();
        std::string getRoot();
        std::string getIndex(const std::string& path);
        bool getAutoIndex();
        long long getClientMaxBodySize();
        StringVector getAllowedMethods();
        std::string getUploadStore();
        std::string getErrorPage(const std::string& status);
        StringVector getHttpRedirection();
        std::map<std::string, std::string>& getCGI();
        bool hasCGI(std::string filename);
        unsigned int getCGI_timeout();
};