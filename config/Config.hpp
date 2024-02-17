/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:06:08 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/17 16:13:04 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Defs.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include "ServerContext.hpp"
#include "SyntaxAnalysis.hpp"
#include "Utils.hpp"
#include "LogicalErrors.hpp"

class Config
{
    private:
        TokensVector tokens;
        ServersVector servers;
        
        void parseSingleValueDirectives(TokensVector::iterator& tok_iter, Context& serverCtx);
        void parseMultiValueDirectives(TokensVector::iterator& tok_iter, Context& serverCtx);
        void parseDirective(TokensVector::iterator& tok_iter, Context& serverCtx);
        void parseLocation(TokensVector::iterator& tok_iter, ServerContext& serverCtx);
        void parseServers();
        void printDirectives(const Context& ctx);
    public:
        Config();
        Config(const std::string& configPath);
        Config(const Config& obj);
        Config& operator=(const Config& obj);
        ~Config();

        const ServersVector& getServers() const;
        ServersVector::const_iterator getServerByHost(const std::string& host) const;
};