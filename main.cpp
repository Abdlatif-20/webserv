/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/01/31 18:13:57 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

int main(int argc, char **argv)
{
    std::string configPath = "webserv.conf";
    try
    {
        if (argc == 2)
            configPath = argv[1];
        else if (argc > 2)
            throw "Error: invalid number of args";
        std::list<Token> tokens = Lexer::tokenize(configPath);

        std::list<Token>::iterator iter = tokens.begin();
        while (iter != tokens.end())
        {
            switch (iter->getType())
            {
            case WORD:
                std::cout << "WORD =>\t\t\t" + iter->getContent() << std::endl;
                break;
            case SEMICOLON:
                std::cout << "SEMICOLON =>\t\t" + iter->getContent() << std::endl;
                break;           
            case OPEN_BRACKET:
                std::cout << "OPEN_BRACKET =>\t\t" + iter->getContent() << std::endl;
                break;           
            case CLOSED_BRACKET:
                std::cout << "CLOSE_BRACKET =>\t" + iter->getContent() << std::endl;
                break;           
            default:
                break;
            }
            iter++;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const char* msg)
    {
        std::cerr << msg << std::endl;
    }
    return 0;
}