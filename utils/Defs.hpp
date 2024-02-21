/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:36:15 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/21 11:08:31 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <utility>
#include <map>
#include <iostream>

class ServerContext;
class Token;
class LocationContext;

typedef std::vector<std::string> StringVector;
typedef std::pair<std::string, StringVector> Directive;
typedef std::map<std::string, StringVector>  DirectivesMap;
typedef std::vector<ServerContext> ServersVector;
typedef std::vector<Token> TokensVector;
typedef std::vector<LocationContext> LocationsVector;
typedef std::pair<std::string, std::string> HostPort;
typedef std::invalid_argument	Fail;

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