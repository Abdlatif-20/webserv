/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:17:03 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/18 13:35:19 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::string Utils::strTrim(const std::string& str, char c)
{
    if (str.empty())
        return "";
    size_t i = str.find_first_not_of(c);
    size_t j = str.find_last_not_of(c);
    if (i == std::string::npos)
        i = 0;
    if (j == std::string::npos)
        j = str.size();
    return str.substr(i, j - i + 1);
}

t_directive Utils::getDirectiveFromTokenName(const std::string& tokenName)
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

void	Utils::setupAddr(sockaddr_in *addr, int port)
{
	std::memset(addr, 0, sizeof(sockaddr_in));
	addr->sin_port = port;
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
}

std::string Utils::getTokenNameFromDirective(t_directive d)
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
