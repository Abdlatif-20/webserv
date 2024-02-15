/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SyntaxAnalysis.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:16:10 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/14 11:13:01 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Lexer.hpp"
#include <sstream>
#include "Defs.hpp"
#include "Utils.hpp"

class SyntaxErrorException : public std::exception
{
    private:
        std::string str;
    public:
        SyntaxErrorException(const std::string& str);
        SyntaxErrorException(const std::string& str, size_t lineIndex);
        const char* what() const throw();
        ~SyntaxErrorException() throw();
};

void checkSyntax(const TokensVector& tokenList);