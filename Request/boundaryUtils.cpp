/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundaryUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-nei <aben-nei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 20:12:18 by aben-nei          #+#    #+#             */
/*   Updated: 2024/03/08 03:31:54 by aben-nei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// function to check if the key exists in the map
std::string& Request::isExist(Map& headers, std::string key)
{
	if (headers.find(key) != headers.end())
		return headers[key];
	return headers[key] = "";
}

// check if the directory exists
bool Request::directoryExists(const char *path)
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
	size_t posFile = line.find("filename=");
	size_t posName = line.find("name=");
	if (posFile != std::string::npos)
	{
		filename = line.substr(posFile + 10);
		posFile = filename.find("\"");
		if (posFile != std::string::npos)
			filename = filename.substr(0, filename.find("\""));
		else
			return (status = BadRequest, requestIscomplete = true, "");
	}
	else if (posName != std::string::npos)
	{
		filename = line.substr(posName + 6);
		posFile = filename.find("\"");
		if (posFile != std::string::npos)
			filename = filename.substr(0, filename.find("\""));
		else
			return (status = BadRequest, requestIscomplete = true, "");
	}
	if (filename.empty())
		return (filename);
	extension = Utils::getFileExtension(filename);
	int random = std::rand() % 1000;
	filename = filename.substr(0, filename.find_last_of("."));
	filename = this->_path + filename + "_" + Utils::intToString(random) + "." + extension;
	return (filename);
}
