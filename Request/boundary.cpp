/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/04/01 18:18:52 by houmanso         ###   ########.fr       */
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

bool	Request::getBoundaryName(String startBoundary, int &file)
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
					std::remove(_pathTmpFile.c_str()), false);
			file = open(boundaryName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
			if (file == -1)
				return (status = InternalServerError, requestIscomplete = true,
					std::remove(_pathTmpFile.c_str()), false);
			_body.erase(0, _body.find("\r\n\r\n") + 4);
		}
		else
			return (status = InternalServerError, requestIscomplete = true,
				std::remove(_pathTmpFile.c_str()), false);
	}
	return true;
}

int	Request::writeInFile(int& file, String eBoundary, String sBoundary)
{
	if (_body.find(eBoundary) != String::npos)
	{
		write(file, _body.c_str(), _body.find(eBoundary) - 2);
		close(file);
		file = -1;
		tmpFile = -1;
		boundaryName.clear();
		requestIscomplete = true;
		_body.clear();
		return 0;
	}
	else if (_body.find(sBoundary) != String::npos)
	{
		write(file, _body.c_str(), _body.find(sBoundary) - 2);
		_body.erase(0, _body.find(sBoundary));
		close(file);
		file = -1;
		boundaryName.clear();
		return -1;
	}
	write(file, _body.c_str(), _body.size());
	_body.clear();
	return 1;
}

// function to parse the boundary and write the actual file
void Request::parseBoundary()
{
	if (!chunkedBoundary)
		createBoundaryTmpFile();
	else
	{
		this->_pathTmpFile = _chunkedName;
		tmpFiles.push_back(_pathTmpFile);
		_BoundaryComplete = true;
	}
	if (!this->_BoundaryComplete)
		return(receivecount++, void());
		// has cgi
	if (bodySize != contentLength && !chunkedBoundary)
		return (status = BadRequest, requestIscomplete = true,
			std::remove(_pathTmpFile.c_str()), void());
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
			if (!getBoundaryName(startBoundary, file))
				return;
		}
		int ret = writeInFile(file, endBoundary, startBoundary);
		if (!ret)
			return (close(fd), removeFiles(), void());
		else if (ret == -1)
			continue;
	}
}
