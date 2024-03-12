/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunkedUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:40:48 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/10 20:49:46 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// function to convert hex to decimal
unsigned int Request::convertToDecimal(String hex)
{
	if (hex.empty())
		return (0);
	unsigned int decimalNumber;

	std::stringstream ss;
	ss << std::hex << hex;
	ss >> decimalNumber;
	return decimalNumber;
}

// function to prepare the length and the name of the file
void	Request::preparLengthAndName(size_t pos, String& length, std::ofstream& file)
{
	if (!_setLength && (pos = _body.find("\r\n")) != String::npos)
	{
		length = _body.substr(0, pos);
		remainingChunkLength = convertToDecimal(length);
		_body = _body.substr(pos + 2);
		_setLength = true;
	}
	if (this->_chunkedName.empty())
	{
		int random = std::rand() % 1000;
		if (_setLength && _chunkedName.empty() && remainingChunkLength)
			_chunkedName = "chunked_" + Utils::intToString(random);
		if (!_chunkedName.empty() && !file.is_open())
			file.open(this->_path + _chunkedName, std::ios::app);
	}
}
