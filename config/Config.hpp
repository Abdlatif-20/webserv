/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:06:08 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/07 19:49:46 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include "ServerContext.hpp"
#include "SyntaxAnalysis.hpp"
#include "ConfigUtils.hpp"

class Config
{
    private:
        TokensVector tokens;
        ServersVector servers;

        Config();
        void parseDirective(const t_directive& d,
            TokensVector::iterator& tok_iter, Context& serverCtx);
        void parseLocation(t_directive& d,
            TokensVector::iterator& tok_iter, ServerContext& serverCtx);
        void parseServers();
        void printDirectives(const Context& ctx);
    public:
        Config(const std::string& configPath);
        Config(const Config& obj);
        Config& operator=(const Config& obj);
        ~Config();
};