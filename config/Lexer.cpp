/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:15:21 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/18 13:34:13 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

TokensVector Lexer::tokens;

Lexer::Lexer()
{

}

/* The `bool Lexer::isSpecialChar(char c)` function is checking if the given character `c` is a special
character. It returns `true` if the character is one of the following: '{', '}', ';', '#', or if it
is a whitespace character. Otherwise, it returns `false`. This function is used in the process of
tokenizing input strings in the Lexer class. */
bool Lexer::isSpecialChar(char c)
{
    if (c == '{' || c == '}' || c == ';' || c == '#' || std::isspace(c))
        return true;
    return false;
}

/* The `void Lexer::getToken(const std::string& line, size_t lineIndex)` function in the `Lexer` class
is responsible for tokenizing a given input line from a file.*/
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

/* The `TokensVector Lexer::tokenize(const std::string& configFile)` function in the `Lexer` class is
responsible for reading a configuration file specified by `configFile`, tokenizing each line of the
file, and returning a vector of tokens. */
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