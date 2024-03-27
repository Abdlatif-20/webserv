/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/27 17:14:27 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::readBytes(int fd, ssize_t& bytesRead)
{
	bytesRead = read(fd, this->buffer, BUFFER_SIZE);
	if (bytesRead == -1)
		return (std::cerr << "Error: could not read chunked file" << std::endl, void());
	_body.append(this->buffer, bytesRead);
	bzero(this->buffer, BUFFER_SIZE);
}

bool	Request::writeInfile(int fdFile)
{
	if (remainingChunkLength && _setLength)
	{
		if (_body.size() < remainingChunkLength)
		{
			write(fdFile, _body.c_str(), _body.size());
			remainingChunkLength -= _body.size();
			_body.clear();
		}
		else
		{
			write(fdFile, _body.c_str(), remainingChunkLength);
			_body.erase(0, remainingChunkLength + 2);
			remainingChunkLength = 0;
			_setLength = false;
			if (!_body.empty())
				return (false);
		}
	}
	return (true);
}

void	Request::parseChunkedBody()
{
	createChunkedTmpFile();
	if (!_chunkedComplete)
		return(receivecount++, void());
	int fd = open(this->_pathTmpFile.c_str(), O_RDONLY);
	if (fd == -1)
		return (status = InternalServerError, requestIscomplete = true, void());
	ssize_t bytesRead = 0;
	int file = preparName();
	if (!file)
		return (close(fd), void());
	String length = "";
	bytesRead = read(fd, this->buffer, BUFFER_SIZE);
	_body.append(this->buffer, bytesRead);
	while (bytesRead)
	{
		if (!_setLength)
			preparLength(length);
		if (!remainingChunkLength && _setLength)
			return(bodyDone = true, close(file), requestIscomplete = true, close(fd),
				std::remove(this->_pathTmpFile.c_str()), void());
		if (!writeInfile(file))
			continue;
		readBytes(fd, bytesRead);
		if (!bytesRead)
			break;
	}
}
