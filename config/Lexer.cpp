/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:15:21 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/08 14:19:55 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

TokensVector Lexer::tokens;

Lexer::Lexer()
{

}

bool Lexer::isSpecialChar(char c)
{
    if (c == '{' || c == '}' || c == ';' || c == '#' || std::isspace(c))
        return true;
    return false;
}

void Lexer::getToken(const std::string& line, size_t lineIndex)
{
    std::string s = Utils::strTrim(line, ' ');
    std::string _token;
    size_t i = 0;
    while (s[i])
    {
        while (s[i] && !isSpecialChar(s[i]))
            _token += s[i++];
        if (!_token.empty())
            tokens.push_back(Token(_token, WORD, lineIndex));
        _token.clear();
        if (s[i] == '{')
            tokens.push_back(Token("{", OPEN_BRACKET, lineIndex));
        else if (s[i] == '}')
            tokens.push_back(Token("}", CLOSED_BRACKET, lineIndex));
        else if (s[i] == ';')
            tokens.push_back(Token(";", SEMICOLON, lineIndex));
        else if (s[i] == '#')
            break;
        i++;
    }
}

TokensVector Lexer::tokenize(const std::string& configFile)
{
    std::ifstream ifs(configFile);
    std::string line;
    size_t lineIndex = 1;

    if (!ifs.good())
        throw std::runtime_error("Error while opening config_file");
    while (std::getline(ifs, line))
    {
        getToken(line, lineIndex);
        lineIndex++;
    }
    return tokens;
}