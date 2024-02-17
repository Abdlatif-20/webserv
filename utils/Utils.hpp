/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/17 16:55:55 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include <string>

namespace Utils
{
    std::string strTrim(const std::string& str, char c);
    t_directive getDirectiveFromTokenName(const std::string& tokenName);
    std::string getTokenNameFromDirective(t_directive d);
    std::string getDefaultErrorPage(const std::string& status);
};