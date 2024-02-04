/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:06:06 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/04 15:53:42 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{

}

Config::Config(const std::string& configPath)
{
    tokens = Lexer::tokenize(configPath);
    parseServers();
    ServersVector::iterator s_iter = servers.begin();
    DirectivesMap::const_iterator d_iter = s_iter->getDirectives().cbegin();
    while (d_iter != s_iter->getDirectives().cend())
    {
        std::cout << d_iter->first << ": ";
        std::vector<std::string>::const_iterator i = d_iter->second.cbegin();
        while (i != d_iter->second.cend())
        {
            std::cout << *i << " ";
            i++;
        }
        std::cout << std::endl;
        d_iter++;
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

Config::t_directive Config::getDirectiveFromTokenName(const std::string& tokenName)
{
    std::string tokens[10] = 
    {
        "root", "index", "auto_index", "error_page", "client_max_body_size",
        "allowed_methods", "listen", "server_name", "return", "location"
    };
    for (int i = 0; i < 10; i++)
        if (tokenName == tokens[i])
            return static_cast<t_directive>(i);
    return UNKNOWN;
}

void Config::parseLocation(const t_directive& d, TokensVector::iterator& tok_iter)
{
    if (d == LOCATION)
    {
        tok_iter++;
        LocationContext locationCtx(tok_iter->getContent()); /*Create location object with -prefix-*/
        while (tok_iter != tokens.end() && tok_iter->getType() != CLOSED_BRACKET)
        {
            tok_iter++;
        }
    }
}

void Config::parseDirective(const t_directive& d,
    TokensVector::iterator& tok_iter, ServerContext& serverCtx)
{
    std::string key = tok_iter->getContent();
    std::vector<std::string> value;
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
                t_directive d = getDirectiveFromTokenName(tok_iter->getContent());
                parseLocation(d, tok_iter);
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