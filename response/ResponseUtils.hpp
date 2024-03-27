/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:38:28 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/27 01:18:48 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"

class ResponseErrorException : public std::exception
{
    public:
        int status;
        ResponseErrorException() { };
        ResponseErrorException(int _status)
        {
            status = _status;
        }
        ~ResponseErrorException() throw() { };
};

std::string getRangeUnit(const std::string& rangeHeader);
std::string getRangeBytes(const std::string& rangeHeader);
ssize_t getStartOffset(const std::string& range);
ssize_t getEndOffset(const std::string& range);