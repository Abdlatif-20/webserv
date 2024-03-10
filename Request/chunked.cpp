/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 16:42:03 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/10 20:49:59 by aben-nei         ###   ########.fr       */
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
		if (_body.size() < remainingChunkLength
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
