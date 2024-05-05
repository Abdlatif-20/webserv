/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 11:06:08 by mel-yous          #+#    #+#             */
/*   Updated: 2024/05/01 19:12:46 by mel-yous         ###   ########.fr       */
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
        std::string configPath;
        
        void parseSingleValueDirectives(TokensVector::iterator& tok_iter, Context& ctx);
        void parseMultiValueDirectives(TokensVector::iterator& tok_iter, Context& ctx);
        void parseDirective(TokensVector::iterator& tok_iter, Context& ctx);
        void parseLocation(TokensVector::iterator& tok_iter, ServerContext& serverCtx);
        void parseServers();
        void setupDefaultServer();
        void setupDefaultLocation();
        void inheritServerDirectives();
    public:
        Config();
        Config(const std::string& configPath);
        Config(const Config& obj);
        Config& operator=(const Config& obj);
        ~Config();

        ServersVector& getServers();
        const std::string&  getConfigPath();
        void parseConfigFile();
        void resetConfig();
};