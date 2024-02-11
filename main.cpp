/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/10 18:44:19 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Config.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Error: Invalid number of arguments" << std::endl;
		return 1;
	}
	// std::string configPath(av[1]);
	// Config _config(configPath);
	// ServersVector ref = _config.getServers();
	// ServersVector::iterator s_iter = ref.begin();
	// std::cout << s_iter->getLocations().begin()->getPrefix() << std::endl;
	// // std::cout << *(s_iter->getDirectiveByKey("server_name") + 1) << std::endl;
	Utils::runServer(av);
	return 0;
}
