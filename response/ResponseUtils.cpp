/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:38:30 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/27 01:25:39 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseUtils.hpp"

std::string getRangeUnit(const std::string& rangeHeader)
{
    size_t i = rangeHeader.find('=');
    if (i != std::string::npos)
        return rangeHeader.substr(0, i);
    return "";
}

std::string getRangeBytes(const std::string& rangeHeader)
{
    size_t i = rangeHeader.find('=');
    if (i != std::string::npos)
        return rangeHeader.substr(i + 1, rangeHeader.length() - i);
    return "";
}

ssize_t getStartOffset(const std::string& range)
{
    size_t i = range.find('-');
    std::string startOffset;
    if (i != std::string::npos)
    {
        startOffset = range.substr(0, i);
        if (startOffset.find_first_not_of("0123456789") != std::string::npos)
            throw ResponseErrorException(RequestedRangeNotSatisfiable);
        return std::atoll(startOffset.c_str());
    }
    throw ResponseErrorException(RequestedRangeNotSatisfiable); 
    return 0;
}

ssize_t getEndOffset(const std::string& range)
{
    size_t i = range.find('-');
    std::string endOffset;
    if (i != std::string::npos)
    {
        endOffset = range.substr(i + 1, range.length() - i);
        if (endOffset.empty())
            return -1;
        if (endOffset.find_first_not_of("0123456789") != std::string::npos)
            throw ResponseErrorException(RequestedRangeNotSatisfiable);
        return std::atoll(endOffset.c_str());
    }
    return -1;
}