/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/25 20:08:18 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Core.hpp"

int main(int argc, char **argv)
{
    std::string configPath = "webserv.conf";
    try
    {
        if (argc == 2)
            configPath = argv[1];
        else if (argc > 2)
            throw "Error: invalid number of args";
        Config _config(configPath);
        Core _core(_config);
        _core.startWorking();
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
