/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:47:07 by mel-yous          #+#    #+#             */
/*   Updated: 2024/01/28 17:14:47 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

Lexer::Lexer()
{
    content = "";
    type = none;
}

Lexer::Lexer(const std::string& content, t_type type)
{
    this->content = content;
    this->type = type;
}

Lexer::Lexer(const Lexer& obj)
{
    *this = obj;
}

Lexer& Lexer::operator=(const Lexer& obj)
{
    if (this == &obj)
        return *this;
    content = obj.content;
    type = obj.type;
    return *this;
}

Lexer::~Lexer()
{

}