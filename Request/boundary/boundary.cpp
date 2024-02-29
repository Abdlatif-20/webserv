/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/28 19:52:12 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

std::string& isExist(std::map<std::string, std::string>& headers, std::string key)
{
	if (headers.find(key) != headers.end())
		return headers[key];
	return headers[key] = "";
}

// function to parse the content type
void Request::parseContentType()
{
	if (_headers["content-type"].find("boundary") == std::string::npos)
		_status = BadRequest;
	else if (isExist(_headers, "content-length") == "")
		_status = LengthRequired;
	_contentLength = Utils::stringToInt(_headers["content-length"]);
}

// check if the directory exists
bool directoryExists(const char *path)
{
	struct stat st;
	if (!stat(path, &st))
	{
		if (S_ISDIR(st.st_mode))
			return true; // Directory exists
	}
	return false; // Directory does not exist
}

// function to prepare the filename and return it
std::string Request::prepareFileName(std::string line)
{
	std::string filename = "";
	std::string extension = "";
	std::string path = "";
	size_t posFile = line.find("filename=");
	size_t posName = line.find("name=");
	ServersVector ref = _config.getServers();
	path = ref[0].getUploadStore();
	if (!directoryExists(path.c_str()))
		mkdir(path.c_str(), 0777);
	path += "/";
	if (posFile != std::string::npos)
	{
		filename = line.substr(posFile + 10);
		posFile = filename.find("\"");
		if (posFile != std::string::npos)
			filename = filename.substr(0, filename.find("\""));
		else
			return (_status = BadRequest, "");
	}
	else if (posName != std::string::npos)
	{
		filename = line.substr(posName + 6);
		posFile = filename.find("\"");
		if (posFile != std::string::npos)
			filename = filename.substr(0, filename.find("\""));
		else
			return (_status = BadRequest, "");
	}
	if (filename.empty())
		return (filename);
	extension = filename.substr(filename.find_last_of(".") + 1);
	int random = std::rand() % 1000;
	filename = filename.substr(0, filename.find_last_of("."));
	filename = path + filename + "_" + Utils::intToString(random) + "." + extension;
	return (filename);
}

// function to parse the boundary and write the actual file
void Request::parseBoundary()
{
	std::ofstream file;
	std::string randomStr = Utils::intToString(std::rand() % 1000);
	std::string firstBoundary = "--" + _headers["content-type"].substr(30);
	std::string lastBoundary = firstBoundary + "--";
	size_t pos;

	std::cout <<"_body: " << _body << std::endl;
	if (!multipart)
		_sizeBoundary += _body.size();
	if (!_boundaryName.size())
	{
		_boundaryName = prepareFileName(_body);
		if (_boundaryName == "")
			return(_status = BadRequest, void());
		pos = _body.find("\r\n\r\n");
		if (pos != std::string::npos)
			_body = _body.substr(pos + 4);
		else
			_body = "";
		multipart = false;
	}
	if (!file.is_open())
	{
		file.open(_boundaryName, std::ios::app);
		if (!file.is_open())
			return (_status = BadRequest, void());
		if (_sizeBoundary <= _contentLength)
		{
			if (_body.find(lastBoundary) == std::string::npos)
			{
				if (_body.find(firstBoundary) != std::string::npos)
				{
					file << _body.substr(0, _body.find(firstBoundary) - 2);
					_body = _body.substr(_body.find(firstBoundary) - 2);
					file.close();
					_boundaryName = "";
					multipart = true;
					Request::parseBoundary();
				}
				else
				{
					pos = _body.find("\r\n\r\n");
					if (pos != std::string::npos)
						_body = _body.substr(pos + 4);
					file << _body;
				}
				_receivecount++;
			}
			else
			{
				file << _body.substr(0, _body.find(lastBoundary) - 2);
				file.close();
				if (_sizeBoundary != _contentLength)
				{
					_sizeBoundary = 0;
					std::remove(_boundaryName.c_str());
					return (_status = BadRequest, void());
				}
				_bodyDone = true;
				_sizeBoundary = 0;
			}
		}
		else
		{
			_sizeBoundary = 0;
			std::remove(_boundaryName.c_str());
			return (_status = BadRequest, void());
		}
	}
	
	_receivecount++;
}
