/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/13 15:05:51 by houmanso         ###   ########.fr       */
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
			throw Fail("Error: invalid number of args");
		Config _config(configPath);
		Server(_config);
	}
	catch(const std::exception& e)
	{
		std::cerr << "ERROR : " << e.what() << '\n';
	}
	return 0;
}