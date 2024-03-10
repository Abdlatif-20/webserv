/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentLength.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:44:54 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/10 20:49:13 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to parse the content length
void	Request::parseContentLength()
{
	if (_headers["content-length"].find_first_not_of("0123456789") != String::npos)
		return (status = BadRequest, requestIscomplete = true, void());
	if (Utils::stringToInt(_headers["content-length"]) > MAX_BODY_SIZE)
		return (status = RequestEntityTooLarge, requestIscomplete = true, void());
	contentLength = Utils::stringToInt(_headers["content-length"]);
}

//function to parse the content length and write the body to a file
void	Request::ContentLength()
{
	String randomStr = Utils::intToString(std::rand() % 1000);
	std::ofstream file;
	String path = requestLine["path"];
	String extension = "";
	
	if (contentLength != _body.size())
		return (status = BadRequest, requestIscomplete = true, void());
	if (!file.is_open())
	{
		if (path == "/")
			file.open(this->_path + "body" + randomStr + ".txt", std::ios::app);
		else
		{
			path = path.substr(1);
			size_t pos = path.find_last_of(".");
			if (pos != String::npos)
				extension = path.substr(pos + 1);
			path = path.substr(0, pos);
			file.open(this->_path + path + randomStr + "." + extension, std::ios::app);
		}
		if (!file.is_open())
			return (status = BadRequest, requestIscomplete = true, void());
	}
	if (bodyDone == false)
	{
		file << _body;
		contentLength -= _body.size();
		receivecount++;
	}
	if (contentLength == 0)
	{
		file.close();
		bodyDone = true;
		requestIscomplete = true;
	}
}
