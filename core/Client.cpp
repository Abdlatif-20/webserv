/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 19:51:06 by houmanso          #+#    #+#             */
/*   Updated: 2024/02/15 17:44:44 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void)
{
}

Client::Client(const Client __unused &cpy)
{
	
}

Client	&Client::operator=(const Client &cpy)
{
	if (this != &cpy)
	{
		// ...
	}
	return (*this);
}

Client::~Client(void)
{
	
}
