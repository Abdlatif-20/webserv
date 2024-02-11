/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:17:03 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/04 17:43:49 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::string Utils::strTrim(const std::string& str, char c)
{
    if (str.empty())
        throw "Error: empty string";
    size_t i = str.find_first_not_of(c);
    size_t j = str.find_last_not_of(c);
    if (i == std::string::npos)
        i = 0;
    if (j == std::string::npos)
        j = str.size();
    return str.substr(i, j - i + 1);
}
