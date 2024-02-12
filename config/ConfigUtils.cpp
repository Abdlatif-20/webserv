/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:13:18 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/12 18:15:18 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigUtils.hpp"

t_directive ConfigUtils::getDirectiveFromTokenName(const std::string& tokenName)
{
    std::string tokens[11] = 
    {
        "root", "index", "auto_index", "error_page", "client_max_body_size",
        "allowed_methods", "listen", "server_name", "return", "location", "upload_store"
    };
    for (int i = 0; i < 11; i++)
        if (tokenName == tokens[i])
            return static_cast<t_directive>(i);
    return UNKNOWN;
}

std::string ConfigUtils::getTokenNameFromDirective(t_directive d)
{
    switch (d)
    {
        case ROOT:
            return "root";
        case INDEX:
            return "index";
        case AUTO_INDEX:
            return "auto_index";
        case ERROR_PAGE:
            return "error_page";
        case CLIENT_MAX_BODY_SIZE:
            return "client_max_body_size";
        case ALLOWED_METHODS:
            return "allowed_methods";
        case LISTEN:
            return "listen";
        case SERVER_NAME:
            return "server_name";
        case RETURN:
            return "return";
        case LOCATION:
            return "location";
        case UPLOAD_STORE:
            return "upload_store";
        default:
            break;
    }
    return "unknown";
}