/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 14:36:15 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/04 16:03:20 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <utility>
#include <map>

class ServerContext;
class Token;

typedef std::pair<std::string, std::vector<std::string> > Directive;
typedef std::map<std::string, std::vector<std::string> >  DirectivesMap;
typedef std::vector<ServerContext> ServersVector;
typedef std::vector<Token> TokensVector;