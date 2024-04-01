/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contentLength.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:44:54 by aben-nei          #+#    #+#             */
/*   Updated: 2024/04/01 18:19:37 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void	Request::getExtenstionOfBody()
{
	String str = _headers["content-type"];
	String format = "";
	size_t pos = 0;
	Map formats;

	formats["plain"] = ".txt";
	formats["html"] = ".html";
	formats["javascript"] = ".js";
	formats["json"] = ".json";
	formats["xml"] = ".xml";

	pos = str.find("/");
	if (pos != String::npos)
	{
		format = str.substr(pos + 1);
		if (formats.find(format) != formats.end())
			rawExtension = formats[format];
	}
}

//function to parse the content length
void	Request::parseContentLength()
{
	if (_headers["content-length"].find_first_not_of("0123456789") != std::string::npos)
	{
		status = BadRequest;
		requestIscomplete = true;
	}
	if (Utils::strToll(_headers["content-length"]) > locationCTX.getClientMaxBodySize())
	{
		status = RequestEntityTooLarge;
		requestIscomplete = true;
	}
	getExtenstionOfBody();
	contentLength = Utils::strToll(_headers["content-length"]);
}

//function to parse the content length and write the body to a file
void	Request::ContentLength()
{
	String randomStr = Utils::intToString(std::rand() % 1000);
	std::ofstream file;
	String path = requestLine["path"];
	String extension = "";
	size_t pos = 0;

	if (contentLength > locationCTX.getClientMaxBodySize())
		return (status = RequestEntityTooLarge, requestIscomplete = true, void());
	if (contentLength != (long long)_body.size())
		return (status = BadRequest, requestIscomplete = true, void());
	if (!file.is_open())
	{
		if (path == "/")
			file.open(this->_path + "body" + randomStr + rawExtension, std::ios::app);
		else
		{
			path = path.substr(1);
			pos = path.find_last_of(".");
			if (pos != String::npos)
				extension = path.substr(pos + 1);
			path = this->_path + path.substr(0, pos) + "." + extension;
			if (!fileExists(path))
				file.open(path, std::ios::app);
			else
			{
				path = generatePath(path);
				file.open(path, std::ios::app);
			}
		}
		if (!file.is_open())
			return (status = InternalServerError, requestIscomplete = true, void());
	}
	if (bodyDone == false)
	{
		file.write(_body.c_str(), _body.size());
		contentLength -= _body.size();
		receivecount++;
	}
	if (!contentLength)
		return (file.close(), bodyDone = true, requestIscomplete = true, void());
}
