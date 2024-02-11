/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/11 04:06:52 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

//function to parse the content type
void	Request::parseContentType()
{
	if (_headers["content-type"].find("boundary") == std::string::npos)
	{
		_status = BadRequest;
		throw InvalidRequest("Invalid body(Invalid Content-Type)");
	}
}

static std::string pripareFileName(std::string line, bool &initialFile)
{
	std::string filename;
	std::string extension;

	if (line.find("filename") != std::string::npos)
	{
		filename = line.substr(line.find("filename=") + 10,
			line.find_last_of("\"") - line.find("filename=") - 10);
		std::cout << "filename: " << filename << std::endl;
	}
	else if (line.find("name") != std::string::npos)
	{
		filename = line.substr(line.find("name=") + 6,
			line.find_last_of("\"") - line.find("name=") - 6);
		std::cout << "name: " << filename << std::endl;
	}
	initialFile = true;
	extension = filename.substr(filename.find_last_of(".") + 1);
	int random = std::rand() % 1000;
	filename = filename.substr(0, filename.find_last_of("."));
	filename = filename + "_" + Utils::intToString(random) + "." + extension;
	return (filename);
}

void	Request::boundary()
{
	std::ifstream file(_boundaryName);
	bool initialFile = false;
	std::string filename;
	std::string extension;
	std::string line;
	std::map<std::string, std::string> boundaryMap;
	std::string boundary = _headers["content-type"].substr(30);
	std::string boundaryEnd = "--" + boundary + "--";
	bool isComplete = false;

	if (!file.is_open())
	{
		_status = BadRequest;
		throw InvalidRequest("can't open file");
	}
	while (std::getline(file, line))
	{
		// std::cout << "line: " << line << std::endl;
		if (line.find("Content-Disposition") != std::string::npos)
		{
			filename = pripareFileName(line, initialFile);
			continue;
		}
		if (line.find(boundaryEnd) != std::string::npos)
		{
			isComplete = true;
		}
			if (initialFile && !isComplete)
			{
				std::ofstream file(filename, std::ios::app);
				if (!file.is_open())
				{
					_status = BadRequest;
					throw InvalidRequest("can't open file");
				}
				file << line;
			}
	}
}

void	Request::parseBoundary()
{
	std::string randomStr = Utils::intToString(std::rand() % 1000);
	_boundaryName = "/tmp/boundary" + randomStr + ".txt";
	std::ofstream file(_boundaryName, std::ios::app);
	bool isComplete = false;
	std::string StartBoundary = "--" + _headers["content-type"].substr(30);
	std::string EndBoundary = StartBoundary + "--";
	if (!file.is_open())
	{
		_status = BadRequest;
		throw InvalidRequest("can't open file");
	}
	if (_body.find(EndBoundary) == std::string::npos)
	{
		file << _body;
		_receivecount++;
	}
	else
	{
		file << _body.substr(0, _body.find(EndBoundary) + EndBoundary.size() + 2);
		_body = _body.substr(_body.find(EndBoundary) + EndBoundary.size() + 2);
		isComplete = true;
	}
	if (isComplete)
	{
		file.close();
		boundary();
		_bodyDone = true;
		std::cout << "Body parsed" << std::endl;
	}
}