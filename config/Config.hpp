/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:06:08 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/04 15:54:43 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include "ServerContext.hpp"

class Config
{
    private:
        typedef enum e_directive
        {
            ROOT,
            INDEX,
            AUTO_INDEX,
            ERROR_PAGE,
            CLIENT_MAX_BODY_SIZE,
            ALLOWED_METHODS,
            LISTEN,
            SERVER_NAME,
            RETURN,
            LOCATION,
            UNKNOWN
        } t_directive;
        TokensVector tokens;
        ServersVector servers;

        Config();
        t_directive getDirectiveFromTokenName(const std::string& tokenName);
        void parseDirective(const t_directive& d,
            TokensVector::iterator& tok_iter, ServerContext& serverCtx);
        void parseLocation(const t_directive& d, TokensVector::iterator& tok_iter);
        void parseServers();
    public:
        Config(const std::string& configPath);
        Config(const Config& obj);
        Config& operator=(const Config& obj);
        ~Config();
};