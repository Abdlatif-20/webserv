/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:16:42 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/04 17:44:51 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>

namespace Utils
{
	std::string strTrim(const std::string& str, char c);

	// typedefs:

	// exception class for genral errors , fails ,invaled_argment.
	typedef std::invalid_argument	Fail;
};