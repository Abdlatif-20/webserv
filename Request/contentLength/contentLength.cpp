/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentLength.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:44:54 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/29 20:35:46 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to parse the content length
void	Request::parseContentLength()
{
	if (_headers["content-length"].find_first_not_of("0123456789") != std::string::npos)
		status = BadRequest;
	if (Utils::stringToInt(_headers["content-length"]) > MAX_BODY_SIZE)
		status = RequestEntityTooLarge;
	contentLength = Utils::stringToInt(_headers["content-length"]);
}

//function to parse the content length and write the body to a file
void	Request::ContentLength()
{
	std::string randomStr = Utils::intToString(std::rand() % 1000);
	std::ofstream file;
	std::string path = requestLine["path"];
	std::string extension = "";
	
	if (!file.is_open())
	{
		if (path == "/")
			file.open("body" + randomStr + ".txt", std::ios::app);
		else
		{
			path = path.substr(1);
			size_t pos = path.find(".");
			if (pos != std::string::npos)
				extension = path.substr(pos + 1);
			path = path.substr(0, pos);
			file.open(path + randomStr + "." + extension, std::ios::app);
		}
		if (!file.is_open())
			status = BadRequest;
	}
	if (bodyDone == false)
	{
		if (contentLength < _body.size())
		{
			file << _body.substr(0, contentLength);
			_body = _body.substr(contentLength);
			contentLength = 0;
		}
		else
		{
			file << _body;
			contentLength -= _body.size();
			receivecount++;
		}
	}
	if (contentLength == 0)
	{
		file.close();
		bodyDone = true;
	}
}
