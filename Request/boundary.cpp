/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/29 17:03:30 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// function to parse the content type
void Request::parseContentType()
{
	if (_headers["content-type"].find("boundary") == String::npos)
		return (this->status = BadRequest, requestIscomplete = true, void());
	if (isExist(_headers, "content-length") == "")
		return (this->status = LengthRequired, requestIscomplete = true, void());
	contentLength = std::stol(_headers["content-length"]);
}

// function to parse the boundary and write the actual file
void Request::parseBoundary()
{
	if (!multipart)
		createBoundaryTmpFile();
	else
	{
		this->_pathTmpFile = _chunkedName;
		tmpFiles.push_back(_pathTmpFile);
		_BoundaryComplete = true;
	}
	if (!this->_BoundaryComplete)
		return(receivecount++, void());
	if (bodySize != contentLength && !multipart)
		return (status = BadRequest, requestIscomplete = true, std::remove(_pathTmpFile.c_str()), void());
	int fd = open(this->_pathTmpFile.c_str(), O_RDONLY);
	if (fd == -1)
		return (status = InternalServerError, requestIscomplete = true, void());
	ssize_t bytesRead = 0;
	String boundary = _headers["content-type"].substr(30);
	String startBoundary = "--" + boundary;
	String endBoundary = startBoundary + "--";
	int file = -1;
	while (true)
	{
		readBytes(fd, bytesRead);
		if (!bytesRead)
			break;
		if (boundaryName.empty())
		{
			size_t pos = _body.find(startBoundary);
			if (pos != String::npos)
			{
				_body.erase(0, pos + startBoundary.size());
				pos = _body.find("filename=");
				if (pos == String::npos)
					pos = _body.find("name=");
				if (pos != String::npos)
				{
					boundaryName = prepareFileName(_body.substr(pos));
					if (boundaryName.empty())
						return (status = InternalServerError, requestIscomplete = true,
							std::remove(_pathTmpFile.c_str()), void());
					file = open(boundaryName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
					if (file == -1)
						return (status = InternalServerError, requestIscomplete = true,
							std::remove(_pathTmpFile.c_str()), void());
					_body.erase(0, _body.find("\r\n\r\n") + 4);
				}
				else
					return (status = InternalServerError, requestIscomplete = true,
						std::remove(_pathTmpFile.c_str()), void());
			}
		}
		if (_body.find(endBoundary) != String::npos)
		{
			write(file, _body.c_str(), _body.find(endBoundary) - 2);
			close(file);
			close(fd);
			file = -1;
			tmpFile = -1;
			boundaryName.clear();
			requestIscomplete = true;
			_body.clear();
			break;
		}
		else if (_body.find(startBoundary) != String::npos)
		{
			write(file, _body.c_str(), _body.find(startBoundary) - 2);
			close(file);
			file = -1;
			boundaryName.clear();
			_body.erase(0, _body.find(startBoundary));
			continue;
		}
		write(file, _body.c_str(), _body.size());
		_body.clear();
	}
	removeFiles();
}
