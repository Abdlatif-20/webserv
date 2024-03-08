/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/08 02:04:41 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::parseChunkedBody()
{
	std::string length;
	static std::ofstream file;
	size_t pos = 0;

	preparLengthAndName(pos, length, file);
	if (!remainingChunkLength)
		return(bodyDone = true, file.close(), requestIscomplete = true, void());
	else if (remainingChunkLength)
	{
		if (_body.size() < (size_t)remainingChunkLength
			&& _body.find("\r\n") == std::string::npos)
			{
			file.write(_body.c_str(), _body.size());
			// std::cout << "remainingChunkLength: " << remainingChunkLength << std::endl;
			// remainingChunkLength -= _body.size();
			// std::cout << "A - remainingChunkLength: " << remainingChunkLength << std::endl;
			}
		else if (_body.size() > (size_t)remainingChunkLength
			&& _body.find("\r\n") == std::string::npos)
			{
			file.write(_body.c_str(), remainingChunkLength);
			_body = _body.substr(remainingChunkLength);
			remainingChunkLength = 0;
			}
		else
		{
			std::string beflength = "";
			pos = _body.find("\r\n");
			if (pos != std::string::npos)
			{
				// std::cout <<"B _body: " << _body << std::endl;
				beflength = _body.substr(0, pos);
				// std::cout << "beflength: " << beflength << std::endl;
				file.write(_body.c_str(), pos);
				_body = _body.substr(pos + 2);
				// if (_body.find_first_not_of("0123456789") != std::string::npos)
				// {
				// 	pos = _body.find("\r\n");
				// 	if (pos != std::string::npos)
				// 	{
				// 		file.write(_body.c_str(), pos);
				// 		_body = _body.substr(pos + 2);
				// 		_setLength = false;
				// 		Request::parseChunkedBody();
				// 	}
				// 	else
				// 	{
				// 		file.write(_body.c_str(), _body.size());
				// 		remainingChunkLength -= _body.size();
				// 		_setLength = false;
				// 		return;
				// 	}
				// }
				// std::cout << "A _body: " << _body << std::endl;
				// remainingChunkLength -= pos;
				_setLength = false;
				Request::parseChunkedBody();
			}
		}
	}
	receivecount++;
}
