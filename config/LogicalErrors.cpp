/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogicalErrors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:14:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/04/30 12:10:40 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogicalErrors.hpp"

LogicalErrorException::LogicalErrorException(const std::string& str)
{
    this->str = "Logical error, " + str;
}

LogicalErrorException::~LogicalErrorException() throw()
{

}

const char* LogicalErrorException::what() const throw()
{
    return str.c_str();
}

static bool isNumber(const std::string& str)
{
    size_t i = str.find_first_not_of("0123456789");
    if (i != std::string::npos)
        return false;
    return true;
}

static void isValidPort(const std::string& str)
{
    if (str.empty())
        throw LogicalErrorException("no port in `listen` directive");
    if (!isNumber(str))
            throw LogicalErrorException("invalid port `" + str + "` of the listen directive");
    long long port = Utils::strToll(str);
    if (port <= 0 || port > 65536)
        throw LogicalErrorException("the port is out of range");
}

static void checkListen(const std::string& str)
{
    size_t i = str.find(':');
    std::string port;
    if (i != std::string::npos)
    {
        std::string host = str.substr(0, i);
        if (host.empty())
            throw LogicalErrorException("no port in `listen` directive");
        port = str.substr(i + 1, str.length() - i);
    }
    else
        port = str;
    isValidPort(port);
}

static void checkAutoIndex(Context& ctx)
{
    DirectivesMap::iterator it = ctx.getDirectives().find("auto_index");
    if (it != ctx.getDirectives().end() && !(*it->second.begin() == "on"
        || *it->second.begin() == "off" || *it->second.begin() == "ON"
        || *it->second.begin() == "OFF"))
        throw LogicalErrorException("invalid value for `auto_index` directive");
}

static bool checkLongLongOverflow(const std::string& str)
{
    size_t i = 0;
    size_t j = 0;
    unsigned long long num;
    std::stringstream ss;
    for (; i < str.length() && str[i] == '0'; i++) { }
    for (; i < str.length() && std::isdigit(str[i]); i++)
        j++;
    ss << str;
    ss >> num;
    if (j > 19 || num > 9223372036854775807)
        return true;
    return false;
}

static void checkMaxClientBodySize(Context& ctx)
{
    DirectivesMap::iterator it = ctx.getDirectives().find("client_max_body_size");
    if (it != ctx.getDirectives().end())
    {
        std::string value = *it->second.begin();
        size_t i = value.find_first_not_of("0123456789");
        if (i != std::string::npos)
        {
            if (i != value.length() - 1 || (value[i] != 'K' && value[i] != 'M' && value[i] != 'G') || checkLongLongOverflow(value))
                throw LogicalErrorException("invalid value for `client_max_body_size` directive");
        }
    }
}

static void checkAllowedMethods(Context& ctx)
{
    DirectivesMap::iterator it = ctx.getDirectives().find("allowed_methods");
    if (it != ctx.getDirectives().end())
    {
        StringVector::iterator vec_it = it->second.begin();
        while (vec_it != it->second.end())
        {
            if (*vec_it != "GET" && *vec_it != "POST" && *vec_it != "DELETE")
                throw LogicalErrorException("invalid value for `allowed_methods` directive");
            vec_it++;
        }
    }
}

static bool checkIntOverflow(const std::string& str)
{
    size_t i = 0;
    size_t j = 0;
    for (; i < str.length() && str[i] == '0'; i++) { }
    for (; i < str.length(); i++)
        j++;
    if (j > 10 || std::atol(str.c_str()) > 2147483647)
        return true;
    return false;
}

static void checkErrorPages(Context& ctx)
{
    if (ctx.getErrorPages().size() > 0)
    {
        std::map<std::string, std::string>::iterator it = ctx.getErrorPages().begin();
        while (it != ctx.getErrorPages().end())
        {
            if (isNumber(it->first))
            {
                std::string key = it->first;
                int status = std::atoi(key.c_str());
                if (checkIntOverflow(key) || status < 400 || status > 507)
                    throw LogicalErrorException("invalid status code for `error_page` directive");
            }
            it++;
        }
    }
}

static void checkReturn(Context& ctx)
{
    DirectivesMap::iterator it = ctx.getDirectives().find("return");
    if (it != ctx.getDirectives().end())
    {
        StringVector::iterator vec_it = it->second.begin();
        int code = std::atoi((*vec_it).c_str());
        if (!isNumber(*vec_it) || checkIntOverflow(*vec_it) || code < 300 || code > 307)
            throw LogicalErrorException("invalid value for `return` directive");
    }
}

static void checkCGI_timeout(Context& ctx)
{
    DirectivesMap::iterator it = ctx.getDirectives().find("cgi_max_timeout");
    if (it != ctx.getDirectives().end())
    {
        StringVector::iterator vec_it = it->second.begin();
        if (!isNumber(*vec_it) || checkIntOverflow(*vec_it))
            throw LogicalErrorException("invalid value for `cgi_max_timeout` directive");
    }
}

static void checkHttpDirectives(Context& ctx)
{
    checkAutoIndex(ctx);
    checkMaxClientBodySize(ctx);
    checkAllowedMethods(ctx);
    checkErrorPages(ctx);
    checkReturn(ctx);
    checkCGI_timeout(ctx);
}

void checkLogicalErrors(ServersVector& servers)
{
    ServersVector::iterator serv_iter = servers.begin();
    while (serv_iter != servers.end())
    {
        LocationsVector::iterator loc_vector = serv_iter->getLocations().begin();
        while (loc_vector != serv_iter->getLocations().end())
            checkHttpDirectives(*(loc_vector++));
        checkListen(serv_iter->getListen());
        checkHttpDirectives(*serv_iter);
        serv_iter++;
    }
}