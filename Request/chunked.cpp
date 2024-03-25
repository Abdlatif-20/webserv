/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/25 02:31:43 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// if (!_chunkedComplete)
	// {
	// 	if (!fd)
	// 		fd = open("/tmp/chunked", O_CREAT | O_RDWR | O_APPEND, 0666);
	// 	if (fd == -1)
	// 	{
	// 		std::cerr << "Error: " << strerror(errno) << std::endl;
	// 		return;
	// 	}
	// 	write(fd, _body.c_str(), _body.size());
	// 	if (_body.find("0\r\n\r\n") != String::npos)
	// 	{
	// 		_chunkedComplete = true;
	// 	}
	// }
void	Request::parseChunkedBody()
{
	// std::ofstream file2;
	// if (!file2.is_open())
	// 	file2.open("chunkedLog", std::ios::app);
	// file2.write(_body.c_str(), _body.size());
	write(1, _body.c_str(), _body.size());
	String length;
	static std::ofstream file;
	size_t pos = 0;

	if (this->_chunkedName.empty() || !_setLength)
		preparLengthAndName(pos, length, file);
	if (!remainingChunkLength)
		return(bodyDone = true, file.close(), requestIscomplete = true, void());
	else if (remainingChunkLength)
	{
		if (_body.size() < (size_t)remainingChunkLength
			&& _body.find("\r\n") == String::npos)
		{
			file.write(_body.c_str(), _body.size());
			remainingChunkLength -= _body.size();
		}
		else
		{
			String beflength = "";
			pos = _body.find("\r\n");
			if (pos != String::npos)
			{
				beflength = _body.substr(0, pos);
				file.write(_body.c_str(), pos);
				_body = _body.substr(pos + 2);
				_setLength = false;
				Request::parseChunkedBody();
			}
		}
	}
	receivecount++;
}