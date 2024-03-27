/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/27 01:59:42 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::parseChunkedBody()
{
	static int fd = 0;
	if (!fd)
		fd = open("/tmp/chunked", O_CREAT | O_RDWR | O_APPEND, 0666);
	if (fd == -1)
		return (std::cerr << "Error: could not open tmp file" << std::endl,
			std::remove("/tmp/chunked"), void());
	write(fd, _body.c_str(), _body.size());
	if (_body.find("\r\n0\r\n\r\n") != String::npos)
	{
		_body.clear();
		_chunkedComplete = true;
		close(fd);
	}
	if (_chunkedComplete)
	{
		int fd = open("/tmp/chunked", O_RDONLY);
		if (fd == -1)
			return (std::cerr << "Error: not open tmp file" << std::endl, void());
		const int BUFFER_SIZE = 10000;
		char buffer[BUFFER_SIZE];
		bzero(buffer, BUFFER_SIZE);
		ssize_t bytesRead = 0;
		std::string str;
		std::string fileName = this->_path + "chunked_" + Utils::intToString(std::rand() % 1000);
		String length = "";
		size_t pos = 0;
		int file = open(fileName.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
		if (file == -1)
			return (std::cerr << "Error: could not open chunked file" << std::endl, void());
		std::string tmp;
		bytesRead = read(fd, buffer, BUFFER_SIZE);
		str.clear();
		str.append(buffer, bytesRead);
		while (bytesRead)
		{
			if (!_setLength)
			{
				pos = str.find("\r\n");
				if (pos != std::string::npos)
				{
					length = str.substr(0, pos);
					remainingChunkLength = convertToDecimal(length);
					length.clear();
					str = str.substr(pos + 2);
					_setLength = true;
				}
			}
			if (!remainingChunkLength && _setLength)
				return(bodyDone = true, close(file), requestIscomplete = true, close(fd),
					std::remove("/tmp/chunked"), void());
			if (remainingChunkLength && _setLength)
			{
				if (str.size() < (size_t)remainingChunkLength)
				{
					write(file, str.c_str(), str.size());
					remainingChunkLength -= str.size();
					str.clear();
				}
				else
				{
					write(file, str.c_str(), remainingChunkLength);
					str.erase(0, remainingChunkLength + 2);
					remainingChunkLength = 0;
					_setLength = false;
					if (!str.empty())
						continue;
				}
			}
			bytesRead = read(fd, buffer, BUFFER_SIZE);
			if (bytesRead == -1)
				return (std::cerr << "Error: could not read chunked file" << std::endl, void());
			if (!bytesRead)
				break;
			str.append(buffer, bytesRead);
			bzero(buffer, BUFFER_SIZE);
		}
	}
	receivecount++;
}

// void	Request::parseChunkedBody()
// {
// 	String length = "";
// 	static std::ofstream file;
// 	size_t pos = 0;
// 	if (this->_chunkedName.empty() || !_setLength)
// 		preparLengthAndName(pos, length, file);
// 	if (!remainingChunkLength && _setLength)
// 		return(bodyDone = true, file.close(), requestIscomplete = true, void());
// 	if (remainingChunkLength)
// 	{
// 		if (_body.size() <= (size_t)remainingChunkLength)
// 		{
// 			file.write(_body.c_str(), _body.size());
// 			remainingChunkLength -= _body.size();
// 			if (!remainingChunkLength)
// 				_setLength = false;
// 			_body.clear();
// 		}
// 		else
// 		{
// 			file.write(_body.c_str(), remainingChunkLength);
// 			if (_body.size() >= (size_t)remainingChunkLength + 2)
// 				_body = _body.substr(remainingChunkLength + 2);
// 			else
// 				_body = _body.substr(remainingChunkLength);
// 			remainingChunkLength = 0;
// 			_setLength = false;
// 			receivecount++;
// 			parseChunkedBody();
// 		}
// 	}
// 	receivecount++;
// }
