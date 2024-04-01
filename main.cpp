/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/04/01 01:18:54 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Utils.hpp"
#include "Config.hpp"
#include "Core.hpp"

void	printBanner()
{
	std::cout << "\n\033[94m";
	std::cout << "██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗" << std::endl;
	std::cout << "██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║" << std::endl;
	std::cout << "██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║" << std::endl;
	std::cout << "██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝" << std::endl;
	std::cout << "╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ " << std::endl;
	std::cout << " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  " << std::endl;
	std::cout << "\033[0m";
	std::cout << "\033[94mCopyright (c) 2024 \033[0m \033[92m[aben-nei - mel-yous - houmanso]\033[0m\n" << std::endl;
}

int main(int argc, char **argv, char  **env)
{
	printBanner();
	std::string configPath = "webserv.conf";
	try
	{
		Response::setupEnv(env);
		if (argc == 2)
			configPath = argv[1];
		else if (argc > 2)
			throw Fail("invalid number of args");
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
