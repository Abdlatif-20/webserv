/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 21:51:55 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/21 22:48:16 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	_clientStatus = 200;
}

Client::~Client()
{
}

void Client::parseRequest(std::string receivedRequest, char *configPath)
{
	this->request.parseRequest(receivedRequest, configPath);
}
