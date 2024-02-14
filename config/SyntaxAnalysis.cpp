/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SyntaxAnalysis.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 15:16:11 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/14 11:13:01 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SyntaxAnalysis.hpp"

#pragma region "EXCEPTION"

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

#pragma endregion

/* The `countDirectiveArgs` function is used to count the number of arguments for a directive in the
server configuration file.*/
static size_t countDirectiveArgs(const TokensVector& tokens, TokensVector::const_iterator& i)
{
    size_t counter = 0;
    while (i != tokens.end() && i->getType() != SEMICOLON)
    {
        if (i->getType() != WORD)
            throw SyntaxErrorException("unexpected `" + i->getContent() + "` at line: ", i->getLineIndex());
        counter++;
        i++;
    }
    return counter;
}

/* The `checkDirective` function is responsible for checking the syntax of a directive in the server
configuration file. It takes a directive `d`, a vector of tokens `tokens`, and an iterator `i` as
input. */
static void checkDirective(t_directive d, const TokensVector& tokens,
    TokensVector::const_iterator& i)
{
    size_t currLineIndex = i->getLineIndex();
    size_t count = countDirectiveArgs(tokens, ++i);
    switch (d)
    {
        case ROOT: case AUTO_INDEX: case CLIENT_MAX_BODY_SIZE: case LISTEN: case UPLOAD_STORE:
            if (count != 1)
                throw SyntaxErrorException("invalid number of args at line: ", currLineIndex);
            break;
        case INDEX: case SERVER_NAME:
            if (count == 0)
                throw SyntaxErrorException("invalid number of args at line: ", currLineIndex);
            break;
        case ERROR_PAGE: case RETURN:
            if (count != 2)
                throw SyntaxErrorException("invalid number of args at line: ", currLineIndex);
            break;
        case ALLOWED_METHODS:
            if (count == 0 || count > 3)
                throw SyntaxErrorException("invalid number of args at line: ", currLineIndex);
            break;
        default:
            break;
    }
}

/* The `checkLocation` function is responsible for checking the syntax of a location block in the
server configuration file. It takes a vector of tokens and an iterator as input. */
static void checkLocation(const TokensVector& tokens,
    TokensVector::const_iterator& i)
{
    size_t currLineIndex = i->getLineIndex();
    t_directive d;

    if (++i == tokens.end() || i->getType() != WORD)
        throw SyntaxErrorException("expected word after `location` directive at line: ", currLineIndex);
    if (++i == tokens.end())
        throw SyntaxErrorException("expected `{` at line: ", currLineIndex);
    else if (i->getType() != OPEN_BRACKET)
        throw SyntaxErrorException("unexpected `" + i->getContent() + "` at line: ", currLineIndex);
    i++;
    while (i != tokens.end() && i->getType() != CLOSED_BRACKET)
    {
        currLineIndex = i->getLineIndex();
        d = Utils::getDirectiveFromTokenName(i->getContent());
        if (d == LOCATION || d == LISTEN || d == SERVER_NAME || d == UNKNOWN)
            throw SyntaxErrorException("unexpected `" + i->getContent() + "` at line: ", i->getLineIndex());
        checkDirective(d, tokens, i);
        i++;
    }
    if (i == tokens.end() || i->getType() != CLOSED_BRACKET)
        throw SyntaxErrorException("expected `}` at line: ", currLineIndex);
}

/* The `checkServer` function is responsible for checking the syntax of a server block in the server
configuration file. It takes a vector of tokens and an iterator as input. */
static void checkServer(const TokensVector& tokens, TokensVector::const_iterator& i)
{
    size_t currLineIndex;
    t_directive d;

    currLineIndex = (i++)->getLineIndex();
    if (i == tokens.end() || i->getType() != OPEN_BRACKET)
        throw SyntaxErrorException("expected `{` at line: ", currLineIndex);
    i++;
    while (i != tokens.end() && i->getType() != CLOSED_BRACKET)
    {
        currLineIndex = i->getLineIndex();
        d = Utils::getDirectiveFromTokenName(i->getContent());
        if (d == LOCATION)
            checkLocation(tokens, i);
        else if (d == UNKNOWN)
            throw SyntaxErrorException("unknown directive `" + i->getContent() + "` at line: ", currLineIndex);
        else
            checkDirective(d, tokens, i);
        if (i != tokens.end())
            i++;
    }
    if (i == tokens.end() || i->getType() != CLOSED_BRACKET)
        throw SyntaxErrorException("expected `}` at line: ", currLineIndex);
}

/** The `checkSyntax` function takes a vector of tokens as input and checks the syntax of the server
configuration file. It iterates through the tokens and checks for the correct usage of directives
and keywords according to the server configuration syntax. If any syntax error is found, it throws a
`SyntaxErrorException` with a descriptive error message indicating the line number and the specific
error. 
* @param tokens tokens vector
* @return Nothing
*/
void checkSyntax(const TokensVector& tokens)
{
    TokensVector::const_iterator i = tokens.cbegin();
    while (i != tokens.cend())
    {
        if (i->getType() == WORD && i->getContent() == "server")
            checkServer(tokens, i);
        else
            throw SyntaxErrorException("expected `server` keyword at line: ", i->getLineIndex());
        if (i != tokens.end())
            i++;
    }
}