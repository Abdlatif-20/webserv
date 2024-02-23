/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 16:26:01 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/23 00:31:07 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Config.hpp"
// #include "Server.hpp"

int main(int __attribute__((unused)) ac, __attribute__((unused)) char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error: Invalid number of arguments" << std::endl;
		return 1;
	}
	// char beni[10] = "hell\0mkhg";
	// std::string BeniproMax(beni);
	// std::cout << BeniproMax.size() << std::endl;
	// std::cout << BeniproMax.size() << std::endl;
	// char binary[10] = "hell\0mkhg";
	// std::ofstream file("hell", std::ios::binary);
	// file.write(binary, 10);
	Utils::runServer(av);
	return 0;
}
