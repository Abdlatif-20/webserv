/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/05/05 16:04:26 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Utils.hpp"
#include "Config.hpp"
#include "CGI.hpp"
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
	std::cout << "\033[92m[aben-nei - mel-yous - houmanso]\033[0m         \033[94mCopyright (c) 2024 \033[0m \n" << std::endl;
}

int main(int argc, char **argv, char  **env)
{
	printBanner();
	std::string configPath = "webserv.conf";
	try
	{
		CGI::setPath(env);
		if (argc == 2)
			configPath = argv[1];
		else if (argc > 2)
			throw Fail("invalid number of args");
		Config	_config(configPath);
		Core _core;
		while (true)
		{
			_config.parseConfigFile();
			_core.setConfig(_config);
			_core.run();
			_core.resetCore();
			_config.resetConfig();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::exit(EXIT_FAILURE);
	}
	return 0;
}
