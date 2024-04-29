/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:38:28 by mel-yous          #+#    #+#             */
/*   Updated: 2024/04/28 22:46:35 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include <fstream>
#include "Utils.hpp"

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
long long getStartOffset(const std::string& range);
long long getEndOffset(const std::string& range);
ssize_t myRead(std::ifstream& ifs, char* buff, size_t sz);

void clear_folder_or_file(const std::string& path);