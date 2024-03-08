/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/08 03:27:10 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// function to parse the content type
void Request::parseContentType()
{
	if (_headers["content-type"].find("boundary") == std::string::npos)
	{
		status = BadRequest;
		requestIscomplete = true;
	}
	else if (isExist(_headers, "content-length") == "")
	{
		status = LengthRequired;
		requestIscomplete = true;
	}
	contentLength = Utils::stringToInt(_headers["content-length"]);
}

void	removeFiles(Vector files)
{
	for (size_t i = 0; i < files.size(); i++)
		std::remove(files[i].c_str());
}

// function to parse the boundary and write the actual file
void Request::parseBoundary()
{
	std::ofstream file;
	std::string randomStr = Utils::intToString(std::rand() % 1000);
	std::string firstBoundary = "--" + _headers["content-type"].substr(30);
	std::string lastBoundary = firstBoundary + "--";
	size_t pos;

	if (!multipart)
		sizeBoundary += _body.size();
	if (!this->boundaryName.size())
	{
		this->boundaryName = prepareFileName(_body);
		if (this->boundaryName == "")
			return(status = BadRequest, this->requestIscomplete = true, void());
		pos = _body.find("\r\n\r\n");
		if (pos != std::string::npos)
			_body = _body.substr(pos + 4);
		else
			_body = "";
		multipart = false;
	}
	if (!file.is_open())
	{
		file.open(this->boundaryName, std::ios::app);
		if (!file.is_open())
			return (status = BadRequest, this->requestIscomplete = true, void());
		files.push_back(this->boundaryName);
	}
		if (sizeBoundary <= contentLength)
		{
			size_t posLastBoundary = _body.find(lastBoundary);
			size_t posFirstBoundary = _body.find(firstBoundary);
			if (posLastBoundary == std::string::npos)
			{
				if (posFirstBoundary != std::string::npos)
				{
					file << _body.substr(0, posFirstBoundary - 2);
					_body = _body.substr(posFirstBoundary - 2);
					file.close();
					this->boundaryName = "";
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
				receivecount++;
			}
			else
			{
				file << _body.substr(0, posLastBoundary - 2);
				file.close();
				if (sizeBoundary != contentLength)
				{
					sizeBoundary = 0;
					removeFiles(files);
					return (status = BadRequest, this->requestIscomplete = true, void());
				}
				bodyDone = true;
				this->requestIscomplete = true;
				sizeBoundary = 0;
			}
		}
		else
		{
			sizeBoundary = 0;
			removeFiles(files);
			return (status = BadRequest, this->requestIscomplete = true, void());
		}
	
	receivecount++;
}
