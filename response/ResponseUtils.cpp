/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:38:30 by mel-yous          #+#    #+#             */
/*   Updated: 2024/04/28 22:55:41 by mel-yous         ###   ########.fr       */
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

long long getStartOffset(const std::string& range)
{
    size_t i = range.find('-');
    std::string startOffset;
    if (i != std::string::npos)
    {
        startOffset = range.substr(0, i);
        if (startOffset.find_first_not_of("0123456789") != std::string::npos)
            throw ResponseErrorException(RequestedRangeNotSatisfiable);
        return Utils::strToll(startOffset);
    }
    throw ResponseErrorException(RequestedRangeNotSatisfiable); 
    return 0;
}

long long getEndOffset(const std::string& range)
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
        return Utils::strToll(endOffset);
    }
    return -1;
}

ssize_t myRead(std::ifstream& ifs, char* buff, size_t sz)
{
    size_t i = 0;
    char c;
    while (i < sz && ifs.get(c))
    {
        if (ifs.fail())
            return -1;
        buff[i++] = c;
    }
    return i;
}

void clear_folder_or_file(const std::string& path)
{
    if (!Utils::isReadableFile(path))
        throw ResponseErrorException(FORBIDDEN);
    if (!Utils::isDirectory(path))
        std::remove(path.c_str());
    else
    {
        DIR* currentDir = opendir(path.c_str());
        dirent* dp;
        std::string targetFile;
        if (!currentDir)
            throw ResponseErrorException(InternalServerError);
        while ((dp = readdir(currentDir)))
        {
            if (dp->d_name == std::string(".") || dp->d_name == std::string(".."))
                continue;
            targetFile = path + "/" + dp->d_name;
            if (!Utils::isDirectory(targetFile))
                std::remove(targetFile.c_str());
            else
                Utils::deleteFolderContent(targetFile);
        }
        closedir(currentDir);
    }
}