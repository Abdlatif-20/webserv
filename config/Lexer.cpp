/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:15:21 by mel-yous          #+#    #+#             */
/*   Updated: 2024/01/29 19:31:39 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

Lexer::Lexer()
{

}

void Lexer::getToken(const std::string& line, std::list<Token>& tokenList)
{
    std::string s = Utils::strTrim(line, ' ');
    std::string _token;
    size_t i = 0;
    while (s[i])
    {
        while (s[i] && s[i] != '{' && s[i] != '}' && s[i] != ';' && s[i] != ' ')
            _token += s[i++];
        if (!_token.empty())
            tokenList.push_back(Token(_token, WORD));
        _token.clear();
        if (s[i] == '{')
            tokenList.push_back(Token("{", OPEN_BRACKET));
        else if (s[i] == '}')
            tokenList.push_back(Token("}", CLOSED_BRACKET));
        else if (s[i] == ';')
            tokenList.push_back(Token(";", SEMICOLON));
        i++;
    }
}

std::list<Token> Lexer::tokenize(const std::string& configFile)
{
    std::ifstream ifs(configFile);
    std::string line;
    std::list<Token> tokenList;

    if (!ifs.good())
        throw std::runtime_error("Error while opening config_file");
    while (std::getline(ifs, line))
    {
        getToken(line, tokenList);
    }
    
    return tokenList;
}