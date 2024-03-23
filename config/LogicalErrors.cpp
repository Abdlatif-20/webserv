/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogicalErrors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:14:35 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/23 20:52:10 by mel-yous         ###   ########.fr       */
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

static void checkAutoIndex(Context& ctx)
{
    DirectivesMap::iterator it = ctx.getDirectives().find("auto_index");
    if (it != ctx.getDirectives().end() && !(*it->second.begin() == "on"
        || *it->second.begin() == "off" || *it->second.begin() == "ON"
        || *it->second.begin() == "OFF"))
        throw LogicalErrorException("invalid value for `auto_index` directive");
}

static void checkMaxClientBodySize(Context& ctx)
{
    DirectivesMap::iterator it = ctx.getDirectives().find("client_max_body_size");
    if (it != ctx.getDirectives().end() && !isNumber(*it->second.begin()))
        throw LogicalErrorException("invalid value for `client_max_body_size` directive");
}

static void checkAllowedMethods(Context& ctx)
{
    DirectivesMap::iterator it = ctx.getDirectives().find("allowed_methods");
    if (it != ctx.getDirectives().end())
    {
        StringVector::iterator vec_it = it->second.begin();
        while (vec_it != it->second.end())
        {
            if (*vec_it != "GET" && *vec_it != "POST" && *vec_it != "DELETE"
                && *vec_it != "get" && *vec_it != "post" && *vec_it != "delete")
                throw LogicalErrorException("invalid value for `allowed_methods` directive");
            vec_it++;
        }
    }
}

static void checkErrorPages(Context& ctx)
{
    if (ctx.getErrorPages().size() > 0)
    {
        std::vector<StringVector>::iterator it = ctx.getErrorPages().begin();
        while (it != ctx.getErrorPages().end())
        {
            StringVector::iterator vec_it = it->begin();
            while (vec_it != it->end() && isNumber(*vec_it))
            {
                int statu = std::atoi(vec_it->c_str());
                if (statu < 400 || statu > 507)
                    throw LogicalErrorException("invalid value status code `error_page` directive");
                vec_it++;
            }
            if (vec_it != it->end() && ++vec_it != it->end())
                throw LogicalErrorException("invalid value for `error_page` directive");
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
        if (!isNumber(*vec_it) || code < 300 || code > 307)
            throw LogicalErrorException("invalid value for `return` directive");
    }
}

static void checkHttpDirectives(Context& ctx)
{
    checkAutoIndex(ctx);
    checkMaxClientBodySize(ctx);
    checkAllowedMethods(ctx);
    checkErrorPages(ctx);
    checkReturn(ctx);
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