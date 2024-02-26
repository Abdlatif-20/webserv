/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentLength.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:44:54 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/25 18:06:14 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to parse the content length
void	Request::parseContentLength()
{
	if (_headers["content-length"].find_first_not_of("0123456789") != std::string::npos)
		_status = BadRequest;
	if (std::stoi(_headers["content-length"]) > MAX_BODY_SIZE)
		_status = RequestEntityTooLarge;
	_contentLength = std::stoi(_headers["content-length"]);
}

//function to parse the content length and write the body to a file
void	Request::ContentLength()
{

	std::string randomStr = Utils::intToString(std::rand() % 1000);
	std::ofstream file("body" + randomStr + ".txt", std::ios::app);
	if (!file.is_open())
		_status = BadRequest;
	if (_bodyDone == false)
	{
		if (_contentLength < _body.size())
		{
			file << _body.substr(0, _contentLength);
			_body = _body.substr(_contentLength);
			_contentLength = 0;
		}
		else
		{
			file << _body;
			_contentLength -= _body.size();
			_receivecount++;
		}
	}
	if (_contentLength == 0)
	{
		file.close();
		_bodyDone = true;
	}
}
