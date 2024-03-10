/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 23:45:02 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/10 10:55:23 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// function to parse the content type
void Request::parseContentType()
{
	if (_headers["content-type"].find("boundary") == std::string::npos)
		return (this->status = BadRequest, requestIscomplete = true, void());
	if (isExist(_headers, "content-length") == "")
		return (this->status = LengthRequired, requestIscomplete = true, void());
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

	if (!this->multipart)
		this->sizeBoundary += this->_body.size();
	if (!this->boundaryName.size())
	{
		this->boundaryName = prepareFileName(this->_body);
		if (this->boundaryName == "")
			return(this->status = BadRequest, this->requestIscomplete = true, void());
		pos = this->_body.find("\r\n\r\n");
		if (pos != std::string::npos)
			this->_body = this->_body.substr(pos + 4);
		else
			this->_body = "";
		this->multipart = false;
	}
	if (!file.is_open())
	{
		file.open(this->boundaryName, std::ios::app);
		if (!file.is_open())
			return (this->status = BadRequest, this->requestIscomplete = true, void());
		files.push_back(this->boundaryName);
	}
		if (this->sizeBoundary <= contentLength)
		{
			size_t posLastBoundary = this->_body.find(lastBoundary);
			size_t posFirstBoundary = this->_body.find(firstBoundary);
			if (posLastBoundary == std::string::npos)
			{
				if (posFirstBoundary != std::string::npos)
				{
					file << this->_body.substr(0, posFirstBoundary - 2);
					this->_body = this->_body.substr(posFirstBoundary - 2);
					file.close();
					this->boundaryName = "";
					this->multipart = true;
					Request::parseBoundary();
				}
				else
				{
					pos = this->_body.find("\r\n\r\n");
					if (pos != std::string::npos)
						this->_body = this->_body.substr(pos + 4);
					file << this->_body;
				}
			}
			else
			{
				file << this->_body.substr(0, posLastBoundary - 2);
				file.close();
				if (this->sizeBoundary != contentLength)
				{
					this->sizeBoundary = 0;
					removeFiles(files);
					return (this->status = BadRequest, this->requestIscomplete = true, void());
				}
				bodyDone = true;
				this->requestIscomplete = true;
				this->sizeBoundary = 0;
			}
		}
		else
		{
			this->sizeBoundary = 0;
			removeFiles(files);
			return (this->status = BadRequest, this->requestIscomplete = true, void());
		}
	
	this->receivecount++;
}
