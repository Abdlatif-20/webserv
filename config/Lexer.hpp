/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:15:12 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/08 14:19:37 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <exception>
#include "Defs.hpp"
#include "Token.hpp"
#include "Utils.hpp"

class Lexer
{
    private:
        Lexer();
        static bool isSpecialChar(char c);
        static void getToken(const std::string& line, size_t lineIndex);

        static TokensVector tokens;
    public:
        static TokensVector tokenize(const std::string& configFile);
};