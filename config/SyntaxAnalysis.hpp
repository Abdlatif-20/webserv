/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SyntaxAnalysis.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:16:10 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/01 16:16:51 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Lexer.hpp"

void checkSyntax(const std::list<Token>& tokenList);

class SyntaxErrorException : public std::exception
{
    public:
        const char* what() const throw();      
};