/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 14:47:04 by mel-yous          #+#    #+#             */
/*   Updated: 2024/01/28 16:48:47 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

typedef enum e_type
{
    word,
    open_brace,
    closed_brace,
    semicolon,
    space,
    none
} t_type;

class Lexer
{
    private:
        std::string content;
        t_type type;
    public:
        Lexer();
        Lexer(const std::string& content, t_type type);
        Lexer(const Lexer& obj);
        Lexer& operator=(const Lexer& obj);
        ~Lexer();
};