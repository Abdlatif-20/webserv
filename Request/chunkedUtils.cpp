/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunkedUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:40:48 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/27 23:39:40 by mel-yous         ###   ########.fr       */
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
	static int num;
	if (num == INT_MAX - 1)
		num = 0;
	this->_chunkedName = this->_path + "chunked_" + Utils::intToString(num++);
	int file = open(this->_chunkedName.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
	if (file < 0)
		return (status = InternalServerError, requestIscomplete = true, 0);
	return (file);
}

void	Request::createChunkedTmpFile()
{
	static int num;
	if (num == INT_MAX - 1)
		num = 0;
	if(_pathTmpFile.empty())
		_pathTmpFile = "/goinfre/chunked_" + Utils::intToString(num++);
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
