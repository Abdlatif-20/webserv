/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:47:04 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/01 18:22:42 by mel-yous         ###   ########.fr       */
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
        size_t lineIndex;
    public:
        Token();
        Token(const std::string& content, t_type type, size_t lineIndex);
        Token(const Token& obj);
        Token& operator=(const Token& obj);
        ~Token();
        
        const std::string& getContent() const;
        t_type getType() const;
        size_t getLineIndex() const;
};