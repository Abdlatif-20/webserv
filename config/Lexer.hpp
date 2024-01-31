/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:15:12 by mel-yous          #+#    #+#             */
/*   Updated: 2024/01/31 18:09:45 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <list>
#include <iostream>
#include <fstream>
#include <exception>
#include "Token.hpp"
#include "Utils.hpp"

class Lexer
{
    private:
        Lexer();
        static bool isSpecialChar(char c);
        static void getToken(const std::string& line, std::list<Token>& tokenList);
    public:
        static std::list<Token> tokenize(const std::string& configFile);
};