/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/25 04:03:42 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::parseChunkedBody()
{
	String length;
	static std::ofstream file;
	size_t pos = 0;

	if (this->_chunkedName.empty() || !_setLength)
		preparLengthAndName(pos, length, file);
	if (!remainingChunkLength)
		return(bodyDone = true, file.close(), requestIscomplete = true, void());
	else if (remainingChunkLength)
	{
		if (_body.size() < (size_t)remainingChunkLength)
		{
			file.write(_body.c_str(), _body.size());
			remainingChunkLength -= _body.size();
		}
		else
		{
			if (remainingChunkLength > 0)
			{
				file.write(_body.c_str(), remainingChunkLength);
				_body = _body.substr(remainingChunkLength + 2);
			}
			remainingChunkLength = 0;
			_setLength = false;
			parseChunkedBody();
		}
	}
	receivecount++;
}
