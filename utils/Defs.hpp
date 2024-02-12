/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:36:15 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/12 18:13:38 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <utility>
#include <map>

class ServerContext;
class Token;
class LocationContext;

typedef std::vector<std::string> StringVector;
typedef std::pair<std::string, StringVector> Directive;
typedef std::map<std::string, StringVector>  DirectivesMap;
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
    UPLOAD_STORE,
    UNKNOWN
} t_directive;