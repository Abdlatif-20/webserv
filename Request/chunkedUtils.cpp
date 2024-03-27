/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunkedUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:40:48 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/27 17:14:30 by aben-nei         ###   ########.fr       */
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

// function to prepare the length of the chunked body

void	Request::preparLength(String& length)
{
	size_t pos = 0;
	pos = _body.find("\r\n");
	if (pos != std::string::npos)
	{
		length = _body.substr(0, pos);
		remainingChunkLength = convertToDecimal(length);
		length.clear();
		_body.erase(0, pos + 2);
		_setLength = true;
	}
}

int	Request::preparName()
{
	this->_chunkedName = this->_path + "chunked_" + Utils::intToString(std::rand() % 1000);
	int file = open(this->_chunkedName.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
	if (file < 0)
	{
		std::cerr << "Error: could not create chunked file" << std::endl;
		return (0);
	}
	return (file);
}

void	Request::createChunkedTmpFile()
{
	if(_pathTmpFile.empty())
		_pathTmpFile = "/tmp/chunked_" + Utils::intToString(std::rand() % 1000);
	if (tmpFile <= 0)
		tmpFile = open(this->_pathTmpFile.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
	write(tmpFile, _body.c_str(), _body.size());
	std::string chunkeEnd = _body.substr(_body.size() - 5, 5);
	if (chunkeEnd.find("0\r\n\r\n") != std::string::npos)
	{
		_body.clear();
		_chunkedComplete = true;
		close(tmpFile);
	}
}
