/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:13:18 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/05 17:13:59 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigUtils.hpp"

t_directive ConfigUtils::getDirectiveFromTokenName(const std::string& tokenName)
{
    std::string tokens[10] = 
    {
        "root", "index", "auto_index", "error_page", "client_max_body_size",
        "allowed_methods", "listen", "server_name", "return", "location"
    };
    for (int i = 0; i < 10; i++)
        if (tokenName == tokens[i])
            return static_cast<t_directive>(i);
    return UNKNOWN;
}
