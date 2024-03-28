/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunkedUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:40:48 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/28 02:58:56 by aben-nei         ###   ########.fr       */
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
// function to check if the file exists
bool Request::fileExists(const std::string& fileName)
{
	return access(fileName.c_str(), F_OK) != -1;
}

// function to prepare the name of the chunked file
int Request::preparName()
{
	int file;

	while (true)
	{
		this->_chunkedName = this->_path + "chunked_" + Utils::intToString(rand() % 1000);
		if (!fileExists(this->_chunkedName))
		{
			file = open(this->_chunkedName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
			if (file > 0)
				return file;
		}
	}
	return -1;
}

// function to create the chunked file
void Request::createChunkedTmpFile()
{
	if (_pathTmpFile.empty())
		_pathTmpFile = "/goinfre/chunked_" + Utils::intToString(rand() % 1000);
	if (tmpFile < 0)
	{
		while (true)
		{
			if (!fileExists(_pathTmpFile))
			{
				tmpFile = open(_pathTmpFile.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
				break;
			}
			else
				_pathTmpFile = "/goinfre/chunked_" + Utils::intToString(rand() % 1000);
		}
	}
	write(tmpFile, _body.c_str(), _body.size());
	std::string chunkEnd = _body.substr(_body.size() - 5, 5);
	if (chunkEnd.find("0\r\n\r\n") != std::string::npos)
	{
		_body.clear();
		_chunkedComplete = true;
		close(tmpFile);
		tmpFile = -1;
	}
}
