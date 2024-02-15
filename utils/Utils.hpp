/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/15 17:38:00 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include <string>
#include <iostream>
#include "Defs.hpp"

namespace Utils
{
	std::string	strTrim(const std::string& str, char c);
	std::string	getTokenNameFromDirective(t_directive d);

	t_directive	getDirectiveFromTokenName(const std::string& tokenName);
};