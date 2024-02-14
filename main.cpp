/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/14 04:23:05 by aben-nei         ###   ########.fr       */
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
	Utils::runServer(av);
	return 0;
}
