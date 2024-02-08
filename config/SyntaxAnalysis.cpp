/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SyntaxAnalysis.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:16:11 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/07 20:15:13 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SyntaxAnalysis.hpp"

SyntaxErrorException::SyntaxErrorException(const std::string& str)
{
    this->str = "Syntax error, " + str;
}

SyntaxErrorException::SyntaxErrorException(const std::string &str, size_t lineIndex)
{
    std::stringstream ss;
    std::string s;
    ss << lineIndex;
    ss >> s;
    this->str = "Syntax error, " + str + s;
}

SyntaxErrorException::~SyntaxErrorException() throw()
{

}

const char *SyntaxErrorException::what() const throw()
{
    return str.c_str();
}

static int countArgs(const TokensVector &tokens, TokensVector::const_iterator& i, t_type t)
{
    int counter = 0;
    i++;
    while (i != tokens.end() && i->getType() != SEMICOLON && i->getType() != CLOSED_BRACKET && i->getType() != OPEN_BRACKET)
    {
        counter++;
        i++;
    }
    return counter;
}

static void checkDirective(t_directive d, const TokensVector &tokens,
    TokensVector::const_iterator& i)
{
    if (d == ROOT || d == LISTEN || d == CLIENT_MAX_BODY_SIZE || d == AUTO_INDEX)
    {
        if (countArgs(tokens, i, SEMICOLON) != 1)
            throw SyntaxErrorException("invalid number of args for `" + ConfigUtils::getTokenNameFromDirective(d) + "` directive at line: ", i->getLineIndex());
    }
    else if (d == INDEX || d == SERVER_NAME)
    {
        if (countArgs(tokens, i, SEMICOLON) == 0)
            throw SyntaxErrorException("invalid number of args for `" + ConfigUtils::getTokenNameFromDirective(d) + "` directive at line: ", i->getLineIndex());
    }
    else if (d == ERROR_PAGE || d == RETURN)
    {
        if (countArgs(tokens, i, SEMICOLON) != 2)
            throw SyntaxErrorException("invalid number of args for `" + ConfigUtils::getTokenNameFromDirective(d) + "` directive at line: ", i->getLineIndex());
    }
    else if (d == ALLOWED_METHODS)
    {
        int count = countArgs(tokens, i, SEMICOLON);
        if (count == 0 || count > 3)
            throw SyntaxErrorException("invalid number of args for `" + ConfigUtils::getTokenNameFromDirective(d) + "` directive at line: ", i->getLineIndex());
    }
    if (i->getType() != SEMICOLON)
        throw SyntaxErrorException("missing `;` at line: ", i->getLineIndex());
}

static void checkLocation(const TokensVector &tokens,
    TokensVector::const_iterator& i, int& brackets)
{
    if ((i + 1) == tokens.end() || (i + 1)->getType() != WORD)
        throw SyntaxErrorException("expected word after `location` at line: ", i + 1 == tokens.end() ? i->getLineIndex() : (i + 1)->getLineIndex());
    if ((i + 2) == tokens.end() || (i + 2)->getType() != OPEN_BRACKET)
        throw SyntaxErrorException("missing `{` at line: ", i + 1 == tokens.end() ? i->getLineIndex() : (i + 1)->getLineIndex());
    while (i != tokens.end() && i->getType() != OPEN_BRACKET)
        i++;
    // if (countArgs(tokens, i, OPEN_BRACKET) != 1)
    //     throw SyntaxErrorException("invalid number of args for `location` directive at line: ", i->getLineIndex());
    brackets++;
    i++;
    while (i != tokens.end() && i->getType() != CLOSED_BRACKET)
    {
        t_directive d = ConfigUtils::getDirectiveFromTokenName(i->getContent());
        if (!(d == ROOT || d == INDEX || d == AUTO_INDEX
            || d == ERROR_PAGE || d == CLIENT_MAX_BODY_SIZE
            || d == ALLOWED_METHODS || d == RETURN))
            throw SyntaxErrorException("unexpected `" + i->getContent() + "` at line: ", i->getLineIndex());
        checkDirective(d, tokens, i);
        if (i != tokens.end())
            i++;
    }
    if (i != tokens.end() && i->getType() == CLOSED_BRACKET)
        brackets--;
}

void checkSyntax(const TokensVector &tokens)
{
    TokensVector::const_iterator i = tokens.cbegin();
    int brackets = 0;
    if (tokens.empty())
        throw SyntaxErrorException("config file must contain at least one server");
    while (i != tokens.cend())
    {
        if (i->getContent() == "server")
        {
            if (++i == tokens.end()) i--;
            if (i->getType() != OPEN_BRACKET)
                throw SyntaxErrorException("expected `{` at line: ", i->getLineIndex());
            i++;
            brackets++;
            while (i != tokens.end() && i->getType() != CLOSED_BRACKET)
            {
                t_directive d = ConfigUtils::getDirectiveFromTokenName(i->getContent());
                if (d == LOCATION)
                    checkLocation(tokens, i, brackets);
                else if (d == UNKNOWN)
                    throw SyntaxErrorException("unexpected `" + i->getContent() + "` at line: ", i->getLineIndex());
                else
                    checkDirective(d, tokens, i);
                if (i != tokens.end())
                    i++;
            }
        }
        else
            throw SyntaxErrorException("unexpected `" + i->getContent() + "` at line: ", i->getLineIndex());
        if (i != tokens.end())
        {
            if (i->getType() == CLOSED_BRACKET)
                brackets--;
            i++;
        }
    }
    if (brackets != 0)
        throw SyntaxErrorException("each opened bracket should be closed");
}