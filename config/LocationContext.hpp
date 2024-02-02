/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:30:59 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/02 20:28:54 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "ServerContext.hpp"

typedef std::pair<std::string, std::vector<std::string> > Directive;

class LocationContext
{
    private:
        std::string prefix;
        std::map<std::string, std::vector<std::string> > directives;
    public:
        LocationContext();
        LocationContext(const LocationContext& obj);
        LocationContext& operator=(const LocationContext& obj);
        ~LocationContext();

        void setPrefix(const std::string& prefix);
        void addDirective(Directive _directive);
};