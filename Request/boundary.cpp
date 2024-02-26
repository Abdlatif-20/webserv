/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/26 11:18:59 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// function to parse the content type
void Request::parseContentType()
{
	if (_headers["content-type"].find("boundary") == std::string::npos)
		_status = BadRequest;
}

// check if the directory exists
bool directoryExists(const char *path)
{
	struct stat st;
	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			return true; // Directory exists
		}
	}
	return false; // Directory does not exist
}

// function to prepare the filename and return it
std::string Request::prepareFileName(std::string line)
{
	std::string filename;
	std::string extension;
	std::string path;
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
		filename = filename.substr(0, filename.find("\""));
	}
	else if (posName != std::string::npos)
	{
		filename = line.substr(posName + 6);
		filename = filename.substr(0, filename.find("\""));
	}
	// initialFile = true;
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

	if (!_boundaryName.size())
	{
		_boundaryName = prepareFileName(_body);
		pos = _body.find("\r\n\r\n");
		if (pos != std::string::npos)
			_body = _body.substr(pos + 4);
		else
			_body = "";
	}
	if (!file.is_open())
	{
		file.open(_boundaryName, std::ios::app);
		if (!file.is_open())
			return (_status = BadRequest, void());
		if (_body.find(lastBoundary) == std::string::npos)
		{
			if (_body.find(firstBoundary) != std::string::npos)
			{
				file << _body.substr(0, _body.find(firstBoundary) - 2);
				_body = _body.substr(_body.find(firstBoundary) - 2);
				file.close();
				_boundaryName = "";
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
			_bodyDone = true;
		}
	}
	_receivecount++;
}
