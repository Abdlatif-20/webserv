/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:47:07 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/01 19:58:54 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token()
{
    content = "";
    lineIndex = 0;
    type = WORD;
}

Token::Token(const std::string& content, t_type type, size_t lineIndex)
{
    this->content = content;
    this->type = type;
    this->lineIndex = lineIndex;
}

Token::Token(const Token& obj)
{
    *this = obj;
}

Token& Token::operator=(const Token& obj)
{
    if (this == &obj)
        return *this;
    content = obj.content;
    type = obj.type;
    lineIndex = obj.lineIndex;
    return *this;
}

Token::~Token()
{
    
}

const std::string& Token::getContent() const
{
    return content;
}

t_type Token::getType() const
{
    return type;
}

size_t Token::getLineIndex() const
{
    return lineIndex;
}