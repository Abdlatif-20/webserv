/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogicalErrors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:14:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/18 11:23:39 by mel-yous         ###   ########.fr       */
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
    for (size_t i = 0; i < str.length(); i++)
        if (!std::isdigit(str[i]))
            return false;
    return true;
}

static void isValidPort(const std::string& str)
{
    if (str.empty())
        throw LogicalErrorException("no port in `listen` directive");
    if (!isNumber(str))
            throw LogicalErrorException("invalid port `" + str + "` of the listen directive");
    int port = std::atoi(str.c_str());
    if (port < 1024 || port > 65536)
        throw LogicalErrorException("the port is registred or out of range");
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

static void checkAutoIndex(const Context& ctx)
{
    DirectivesMap::const_iterator it = ctx.getDirectives().find("auto_index");
    if (it != ctx.getDirectives().cend() && !(*it->second.cbegin() == "on"
        || *it->second.cbegin() == "off" || *it->second.cbegin() == "ON"
        || *it->second.cbegin() == "OFF"))
        throw LogicalErrorException("invalid value for `auto_index` directive");
}

static void checkMaxClientBodySize(const Context& ctx)
{
    DirectivesMap::const_iterator it = ctx.getDirectives().find("client_max_body_size");
    if (it != ctx.getDirectives().cend() && !isNumber(*it->second.cbegin()))
        throw LogicalErrorException("invalid value for `client_max_body_size` directive");
}

static void checkAllowedMethods(const Context& ctx)
{
    DirectivesMap::const_iterator it = ctx.getDirectives().find("allowed_methods");
    if (it != ctx.getDirectives().cend())
    {
        StringVector::const_iterator vec_it = it->second.cbegin();
        while (vec_it != it->second.cend())
        {
            if (*vec_it != "GET" && *vec_it != "POST" && *vec_it != "DELETE"
                && *vec_it != "get" && *vec_it != "post" && *vec_it != "delete")
                throw LogicalErrorException("invalid value for `allowed_methods` directive");
            vec_it++;
        }
    }
}

static void checkErrorPages(const Context& ctx)
{
    if (ctx.getErrorPages().size() > 0)
    {
        std::vector<StringVector>::const_iterator it = ctx.getErrorPages().cbegin();
        while (it != ctx.getErrorPages().cend())
        {
            StringVector::const_iterator vec_it = it->cbegin();
            while (vec_it != it->cend() && isNumber(*vec_it))
            {
                int statu = std::atoi(vec_it->c_str());
                if (statu < 400 || statu > 507)
                    throw LogicalErrorException("invalid value status code `error_page` directive");
                vec_it++;
            }
            if (vec_it != it->cend() && ++vec_it != it->cend())
                throw LogicalErrorException("invalid value for `error_page` directive");
            it++;
        }
    }
}

static void checkReturn(const Context& ctx)
{
    DirectivesMap::const_iterator it = ctx.getDirectives().find("return");
    if (it != ctx.getDirectives().cend())
    {
        StringVector::const_iterator vec_it = it->second.cbegin();
        if (!isNumber(*vec_it))
            throw LogicalErrorException("invalid value for `return` directive");
    }
}

static void checkHttpDirectives(const Context& ctx)
{
    checkAutoIndex(ctx);
    checkMaxClientBodySize(ctx);
    checkAllowedMethods(ctx);
    checkErrorPages(ctx);
    checkReturn(ctx);
}

void checkLogicalErrors(const ServersVector& servers)
{
    ServersVector::const_iterator serv_iter = servers.cbegin();
    while (serv_iter != servers.cend())
    {
        LocationsVector::const_iterator loc_vector = serv_iter->getLocations().cbegin();
        while (loc_vector != serv_iter->getLocations().cend())
            checkHttpDirectives(*(loc_vector++));
        checkListen(serv_iter->getListen());
        checkHttpDirectives(*serv_iter);
        serv_iter++;
    }
}