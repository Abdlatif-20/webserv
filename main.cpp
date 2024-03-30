/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/29 17:48:08 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Utils.hpp"
#include "Config.hpp"
#include "CGI.hpp"
#include "Core.hpp"

int main(int argc, char **argv, char  ** __unused env)
{
	std::string configPath = "webserv.conf";
	try
	{
		CGI::setPath(env);
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
