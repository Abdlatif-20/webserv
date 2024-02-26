/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/26 02:34:56 by aben-nei         ###   ########.fr       */
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
std::string Request::prepareFileName(std::string line, bool &initialFile)
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
	initialFile = true;
	extension = filename.substr(filename.find_last_of(".") + 1);
	int random = std::rand() % 1000;
	filename = filename.substr(0, filename.find_last_of("."));
	filename = path + filename + "_" + Utils::intToString(random) + "." + extension;
	return (filename);
}

static void	ignoredLines(std::ifstream &file)
{
	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("Content-Type:") != std::string::npos)
			continue;
		else
			break;
	}
}

// function to parse the boundary and write the body to a file
void Request::boundary()
{
	std::ifstream file;
	if (!file.is_open())
		file.open(_boundaryName, std::ios::in | std::ios::binary);
	if (!file.is_open())
		_status = BadRequest;
	bool initialFile = false;
	std::string filename;
	std::string boundary = "--" + _headers["content-type"].substr(30);
	std::string boundaryEnd = boundary + "--";
	std::ofstream ofile;
	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("Content-Disposition") != std::string::npos)
		{
			filename = prepareFileName(line, initialFile);
			ignoredLines(file);
			continue;
		}
		if (line.find(boundary) != std::string::npos)
		{
			if (ofile.is_open())
				ofile.close();
			initialFile = false;
			continue;
		}
		if (line.find(boundaryEnd) != std::string::npos)
		{
			_isComplete = true;
			std::remove(_boundaryName.c_str());
		}
		if (initialFile && !_isComplete)
		{
			if (!ofile.is_open())
			{
				ofile.open(filename, std::ios::out | std::ios::app | std::ios::binary);
				if (!ofile.is_open())
					_status = BadRequest;
			}
			ofile.write(line.c_str(), line.size());
			ofile.write("\n", 1);
		}
	}
	file.close();
}

void Request::parseBoundary()
{
	std::string randomStr = Utils::intToString(std::rand() % 1000);
	_boundaryName = "/tmp/boundary" + randomStr + ".txt";
	std::ofstream file(_boundaryName, std::ios::app);
	bool isComplete = false;
	std::string StartBoundary = "--" + _headers["content-type"].substr(31);
	std::string EndBoundary = StartBoundary + "--";
	if (!file.is_open())
		_status = BadRequest;
	if (_body.find(EndBoundary) == std::string::npos)
	{
		file.write(_body.c_str(), _body.size());
		_receivecount++;
	}
	else
	{
		file.write(_body.c_str(), _body.find(EndBoundary) + EndBoundary.size());
		_body = _body.substr(_body.find(EndBoundary) + EndBoundary.size());
		isComplete = true;
	}
	if (isComplete)
	{
		file.close();
		boundary();
		_bodyDone = true;
	}
}
