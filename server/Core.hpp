/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:21:50 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/15 17:26:03 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
#define CORE_HPP

#include <iostream>
#include "Config.hpp"
#include "Server.hpp"

class Core
{
	private:
		ServersVector	serversConf;
		std::vector<Server>	servers;
	public:
		Core(void);
		Core(const Core& cpy);
		Core(const Config& conf);

		Core&	operator=(const Core& cpy);

		~Core(void);
};

#endif
