/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 17:11:04 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/06 12:04:35 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include <string>

namespace ConfigUtils
{
    t_directive getDirectiveFromTokenName(const std::string& tokenName);
    std::string getTokenNameFromDirective(t_directive d);
};