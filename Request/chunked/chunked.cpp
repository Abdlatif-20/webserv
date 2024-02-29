/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/29 01:37:44 by aben-nei         ###   ########.fr       */
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
	// std::ofstream fileTmp;
	// if (!fileTmp.is_open())
	// 	fileTmp.open("testReq", std::ios::app);
	
	// fileTmp << _body;
	// static int i = 0;
	
	std::string length;
	int random = std::rand() % 1000;
	static std::ofstream file;
	size_t pos = 0;

	if (!_setLength && (pos = _body.find("\r\n")) != std::string::npos)
	{
		length = _body.substr(0, pos);
		_remainingChunkLength = convertToDecimal(length);
		_body = _body.substr(pos + 2);
		_setLength = true;
	}
	if (_setLength && _chunkedName.empty() && _remainingChunkLength)
		_chunkedName = "chunked_" + Utils::intToString(random) + ".txt";
	if (!_chunkedName.empty() && !file.is_open())
		file.open(_chunkedName, std::ios::app);
	if (!_remainingChunkLength)
		return(_bodyDone = true, file.close(), void());
	else if (_remainingChunkLength)
	{
		if (_body.size() < _remainingChunkLength
			&& _body.find("\r\n") == std::string::npos)
			file.write(_body.c_str(), _body.size());
		else if (_body.size() > _remainingChunkLength
			&& _body.find("\r\n") == std::string::npos)
			file.write(_body.c_str(), _remainingChunkLength);
		else
		{
			std::string beflength = "";
			pos = _body.find("\r\n");
			if (pos != std::string::npos)
			{
				beflength = _body.substr(0, pos);
				file.write(_body.c_str(), pos);
				_body = _body.substr(pos + 2);
				_setLength = false;
				Request::parseChunkedBody();
			}
		}
	}
	_receivecount++;
}
