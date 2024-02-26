/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/18 06:35:49 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to parse the chunked body

unsigned int convertToDecimal(std::string hex)
{
	unsigned int decimalNumber;

    std::stringstream ss;
    ss << std::hex << hex;
    ss >> decimalNumber;
	return decimalNumber;
}

void	Request::parseChunkedBody()
{
	std::cout << "parsing chunked body" << std::endl;
	std::string length;
	int random = std::rand() % 1000;
	std::string filename = "chunked_" + Utils::intToString(random) + ".txt";

	length = _body.substr(0, _body.find(CRLF));
	_body.erase(0, _body.find(CRLF) + 2);
	while (length != "0")
	{
		unsigned int len = convertToDecimal(length);
		std::ofstream file(filename, std::ios::app);
		file << _body.substr(0, len);
		_body.erase(0, len + 2);
		length = _body.substr(0, _body.find(CRLF));
		_body.erase(0, _body.find(CRLF) + 2);
		if (!_body.size())
		{
			_receivecount++;
			return ;
		}
	}
	_bodyDone = true;
}
