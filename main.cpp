/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/19 18:44:51 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"

static void __unused showTokens(std::vector<Token>& tokens)
{
    std::vector<Token>::iterator iter = tokens.begin();
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

int main(int argc, char **argv)
{
    std::string configPath = "webserv.conf";
    try
    {
        if (argc == 2)
            configPath = argv[1];
        else if (argc > 2)
            throw "Error: invalid number of args";
        char buffer[1024];
        Config _config(configPath);
        size_t sz = -1;
        ServersVector::const_iterator firstServer = _config.getServers().cbegin();
        Server tcpServer(firstServer->getListen());
        tcpServer.Start();
        tcpServer.Accept();
        while (sz != 0)
        {
            bzero(buffer, 1024);
            sz = tcpServer.Receive(buffer);
            std::cout << buffer;
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