/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 10:40:11 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/02 22:35:57 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "LocationContext.hpp"
#include <utility>

typedef std::pair<std::string, std::vector<std::string> > Directive;

class LocationContext;

class ServerContext
{
    private:
        std::vector<LocationContext> locations;
        std::map<std::string, std::vector<std::string> > directives;
    public:
        ServerContext();
        ServerContext(const ServerContext& obj);
        ServerContext& operator=(const ServerContext& obj);
        ~ServerContext();

        void addDirective(Directive _directive);
        void addLocation(LocationContext location);
};