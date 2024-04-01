/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunkedUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:40:48 by aben-nei          #+#    #+#             */
/*   Updated: 2024/04/01 02:01:36 by aben-nei         ###   ########.fr       */
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

	this->_chunkedName = this->_path + "chunked";
	if (fileExists(_chunkedName))
		_chunkedName = generatePath(_chunkedName);
	file = open(this->_chunkedName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
	if (file > 0)
		return file;
	return -1;
}

String Request::generatePath(String fileName)
{
	while (true)
	{
		if (!fileExists(fileName))
			return fileName;
		else
		{
			size_t pos = fileName.find_last_of(".");
			if (pos != String::npos)
			{
				String extension = fileName.substr(pos);
				fileName = fileName.substr(0, pos) + "_";
				fileName += Utils::intToString(rand() % 1000);
				fileName += extension;
			}
			else
				fileName += "_" + Utils::intToString(rand() % 1000);
		}
	}
	return fileName;
}

// function to create the chunked file
void Request::createChunkedTmpFile()
{
	if (_pathTmpFile.empty())
		_pathTmpFile = "/goinfre/chunked_" + Utils::intToString(rand() % 1000);
	if (tmpFile < 0)
	{
		_pathTmpFile = generatePath(_pathTmpFile);
		tmpFile = open(_pathTmpFile.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
		tmpFiles.push_back(_pathTmpFile);
	}
	if (!fileExists(_pathTmpFile))
		return (status = InternalServerError, requestIscomplete = true, void());
	bodySize += _body.size();
	if (bodySize > locationCTX.getClientMaxBodySize())
		return (removeFiles(), status = RequestEntityTooLarge, requestIscomplete = true, void());
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
