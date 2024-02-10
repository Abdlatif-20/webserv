/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:06:06 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/08 13:51:06 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{

}

void Config::printDirectives(const Context &ctx)
{
    DirectivesMap::const_iterator directive_iter = ctx.getDirectives().cbegin();
    std::cout << "location {" << std::endl;
    while (directive_iter != ctx.getDirectives().cend())
    {
        std::vector<std::string>::const_iterator i = directive_iter->second.cbegin();
        std::cout << " " << directive_iter->first;
        while (i != directive_iter->second.cend())
            std::cout << " " << *(i++);
        std::cout << std::endl;
        directive_iter++;
    }
    std::cout << "}" << std::endl;
}

Config::Config(const std::string& configPath)
{
    tokens = Lexer::tokenize(configPath);
    checkSyntax(tokens);
    parseServers();
    ServersVector::iterator s_iter = servers.begin();
    while (s_iter != servers.end())
    {
        LocationsVector::const_iterator location_iter = s_iter->getLocations().cbegin();
        while (location_iter != s_iter->getLocations().cend())
        {
            std::cout << location_iter->getDirectives().begin()->first << std::endl;
            printDirectives(*location_iter);
            location_iter++;
        }
        s_iter++;
    }
}

Config::Config(const Config& obj)
{
    *this = obj;
}

Config& Config::operator=(const Config& obj)
{
    if (this == &obj)
        return *this;
    servers = obj.servers;
    return *this;
}

Config::~Config()
{

}

void Config::parseLocation(t_directive& d,
    TokensVector::iterator& tok_iter, ServerContext& serverCtx)
{
    if (d == LOCATION)
    {
        tok_iter++;
        LocationContext locationCtx(tok_iter->getContent()); /*Create location object with -prefix-*/
        while (tok_iter != tokens.end() && tok_iter->getType() != CLOSED_BRACKET)
        {
            d = ConfigUtils::getDirectiveFromTokenName(tok_iter->getContent());
            parseDirective(d, tok_iter, locationCtx);
            tok_iter++;
        }
        serverCtx.addLocation(locationCtx);
    }
}

void Config::parseDirective(const t_directive& d,
    TokensVector::iterator& tok_iter, Context& serverCtx)
{
    std::string key = tok_iter->getContent();
    std::vector<std::string> value;
    if (d == LOCATION)
        return ;
    if (d == LISTEN || d == ROOT || d == CLIENT_MAX_BODY_SIZE)
    {
        tok_iter++;
        value.push_back(tok_iter->getContent());
        serverCtx.addDirective(Directive(key, value));
    }
    else if (d == INDEX || d == ERROR_PAGE
        || d == ALLOWED_METHODS || d == SERVER_NAME || d == RETURN)
    {
        tok_iter++;
        while (tok_iter != tokens.end() && tok_iter->getType() != SEMICOLON)
            value.push_back((tok_iter++)->getContent());
        serverCtx.addDirective(Directive(key, value));
    }
}

void Config::parseServers()
{
    TokensVector::iterator tok_iter = tokens.begin();
    while (tok_iter != tokens.end())
    {
        if (tok_iter->getType() == OPEN_BRACKET)
        {
            ServerContext serverCtx;
            while (tok_iter != tokens.end() && tok_iter->getType() != CLOSED_BRACKET)
            {
                t_directive d = ConfigUtils::getDirectiveFromTokenName(tok_iter->getContent());
                parseLocation(d, tok_iter, serverCtx);
                parseDirective(d, tok_iter, serverCtx);
                if (tok_iter != tokens.end())
                    tok_iter++;
            }
            servers.push_back(serverCtx);
        }
        if (tok_iter != tokens.end())
            tok_iter++;
    }
}