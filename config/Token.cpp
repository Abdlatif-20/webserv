/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:47:07 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/18 13:37:52 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token()
{
    content = "";
    lineIndex = 0;
    type = WORD;
}

/* The `Token::Token(const std::string& content, t_type type, size_t lineIndex)` constructor in the
`Token` class is a parameterized constructor that initializes a `Token` object with the provided
content, type, and line index values. It sets the content of the token to the provided string
content, assigns the type of the token to the provided `t_type` enum value, and sets the line index
of the token to the provided line index value. */
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

/* The `const std::string& Token::getContent() const` function is a member function of the `Token`
class it returns the content of the token. */
const std::string& Token::getContent() const
{
    return content;
}

/* The `t_type Token::getType() const` function is a member function of the `Token` class. It is
a const member function that returns the type of the token. The function returns the `type` member
variable of the `Token` object, which represents the type of the token (e.g., WORD, SEMICOLON, etc...).*/
t_type Token::getType() const
{
    return type;
}

/* The `size_t Token::getLineIndex() const` function is a member function of the `Token` class.
It is a const member function that returns the line index of the token. The function returns the
`lineIndex` member variable of the `Token` object, which represents the line index where the token
appears in the config file.*/
size_t Token::getLineIndex() const
{
    return lineIndex;
}