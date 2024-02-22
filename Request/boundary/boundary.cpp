/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/02/22 21:16:13 by aben-nei         ###   ########.fr       */
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

//function to prepare the filename and return it
static std::string pripareFileName(std::string line, bool &initialFile)
{
	std::string filename;
	std::string extension;

	if (line.find("filename") != std::string::npos)
	{
		filename = line.substr(line.find("filename=") + 10,
			line.find_last_of("\"") - line.find("filename=") - 10);
	}
	else if (line.find("name") != std::string::npos)
	{
		filename = line.substr(line.find("name=") + 6,
			line.find_last_of("\"") - line.find("name=") - 6);
	}
	initialFile = true;
	extension = filename.substr(filename.find_last_of(".") + 1);
	int random = std::rand() % 1000;
	filename = filename.substr(0, filename.find_last_of("."));
	filename = filename + "_" + Utils::intToString(random) + "." + extension;
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

//function to parse the boundary and write the body to a file
void	Request::boundary()
{
	std::ifstream file(_boundaryName);
	if (!file.is_open())
	{
		_status = BadRequest;
		throw InvalidRequest("can't open file");
	}
	bool initialFile = false;
	std::string filename;
	std::string beforeName;
	std::string line;
	std::string boundary = _headers["content-type"].substr(31);
	std::string boundaryEnd = "--" + boundary + "--";
	std::ofstream ofile;

	while (std::getline(file, line))
	{
		if (line.find("Content-Disposition") != std::string::npos)
		{
			filename = pripareFileName(line, initialFile);
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
		// std::cout <<"line: " << line << std::endl;
		if (line.find(boundaryEnd) != std::string::npos)
		{
			_isComplete = true;
			// std::remove(_boundaryName.c_str());
		}
		if (initialFile && !_isComplete)
		{
			if (!ofile.is_open())
			{
				ofile.open(filename, std::ios::app | std::ios::binary);
				if (!ofile.is_open())
				{
					_status = BadRequest;
					throw InvalidRequest("can't open file");
				}
			}
			ofile.write(line.c_str(), line.size());
			ofile.write("\n", 1);
			// ofile << line << std::endl;
		}
	}
	file.close();
}

void	Request::parseBoundary()
{
	std::string randomStr = Utils::intToString(std::rand() % 1000);
	_boundaryName = "/tmp/boundary" + randomStr + ".txt";
	std::ofstream file(_boundaryName, std::ios::app);
	bool isComplete = false;
	std::string StartBoundary = "--" + _headers["content-type"].substr(31);
	std::string EndBoundary = StartBoundary + "--";
	if (!file.is_open())
	{
		_status = BadRequest;
		throw InvalidRequest("can't open file");
	}
	if (_body.find(EndBoundary) == std::string::npos)
	{
		// std::cout <<"_body: {" << _body << "}" << std::endl;
		// std::cout <<"EndBoundary:{" << EndBoundary << "}" << std::endl;
		
		file.write(_body.c_str(), _body.size());
		_receivecount++;
	}
	else
	{
		file.write(_body.c_str(), _body.find(EndBoundary) + EndBoundary.size() + 2);
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
/*
---------------131065337970097632542736--
---------------------------131065337970097632542736--
*/