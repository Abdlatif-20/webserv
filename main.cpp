/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/01 11:08:48 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Config.hpp"
#include "Core.hpp"

int main(int argc, char **argv)
{
	std::string configPath = "webserv.conf";
	try
	{
		if (argc == 2)
			configPath = argv[1];
		else if (argc > 2)
			throw Fail("Error: invalid number of args");
		try
		{
			Config	_config(configPath);
			Core core(_config);
			core.run();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		
	}
	catch(const std::exception& e)
	{
		std::cerr << "ERROR : " << e.what() << '\n';
	}
	return 0;
}
