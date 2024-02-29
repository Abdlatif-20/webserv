/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/29 20:55:51 by aben-nei         ###   ########.fr       */
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
		return(bodyDone = true, file.close(), void());
	else if (remainingChunkLength)
	{
		if (_body.size() < remainingChunkLength
			&& _body.find("\r\n") == std::string::npos)
			file.write(_body.c_str(), _body.size());
		else if (_body.size() > remainingChunkLength
			&& _body.find("\r\n") == std::string::npos)
			file.write(_body.c_str(), remainingChunkLength);
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
	receivecount++;
}
