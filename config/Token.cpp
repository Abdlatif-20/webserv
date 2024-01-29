/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:47:07 by mel-yous          #+#    #+#             */
/*   Updated: 2024/01/29 18:14:03 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token()
{
    content = "";
    type = WORD;
}

Token::Token(const std::string& content, t_type type)
{
    this->content = content;
    this->type = type;
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
    return *this;
}

Token::~Token()
{
    
}

const std::string& Token::getContent()
{
    return content;
}

t_type Token::getType()
{
    return type;
}