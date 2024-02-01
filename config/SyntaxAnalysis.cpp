/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SyntaxAnalysis.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:16:11 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/01 16:15:57 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SyntaxAnalysis.hpp"

void checkSyntax(const std::list<Token> &tokenList)
{
    std::list<Token>::const_iterator iter = tokenList.cbegin();
    while (iter != tokenList.cend())
    {
        if (iter->getType() == WORD && iter->getContent() == "server")
        {
            iter++;
            if (iter->getType() != OPEN_BRACKET)
                throw "";
        }
        iter++;
    }
}
