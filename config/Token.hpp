/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:47:04 by mel-yous          #+#    #+#             */
/*   Updated: 2024/01/29 20:23:14 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

typedef enum e_type
{
    WORD,
    SEMICOLON,
    OPEN_BRACKET,
    CLOSED_BRACKET
} t_type;

class Token
{
    private:
        std::string content;
        t_type type;
    public:
        Token();
        Token(const std::string& content, t_type type);
        Token(const Token& obj);
        Token& operator=(const Token& obj);
        ~Token();
        
        const std::string& getContent();
        t_type getType();
};