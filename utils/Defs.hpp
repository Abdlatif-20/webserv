/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:36:15 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/05 17:09:27 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <utility>
#include <map>

class ServerContext;
class Token;
class LocationContext;

typedef std::pair<std::string, std::vector<std::string> > Directive;
typedef std::map<std::string, std::vector<std::string> >  DirectivesMap;
typedef std::vector<ServerContext> ServersVector;
typedef std::vector<Token> TokensVector;
typedef std::vector<LocationContext> LocationsVector;

typedef enum e_directive
{
    ROOT,
    INDEX,
    AUTO_INDEX,
    ERROR_PAGE,
    CLIENT_MAX_BODY_SIZE,
    ALLOWED_METHODS,
    LISTEN,
    SERVER_NAME,
    RETURN,
    LOCATION,
    UNKNOWN
} t_directive;