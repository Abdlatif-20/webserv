/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 16:38:20 by houmanso          #+#    #+#             */
/*   Updated: 2024/04/29 15:46:36 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

std::string	CGI::PATH;

CGI::CGI(void)
{
	request = NULL;
	response = NULL;
	// nothing
}

CGI::CGI(Response *resp, Request *req): serverctx(resp->getServerCtx()), locationctx(resp->getLocationCtx())
{
	request = req;
	response = resp;
}

CGI::CGI(const CGI &cpy)
{
	*this = cpy;
}

CGI &CGI::operator=(const CGI &cpy)
{
	if (this != &cpy)
	{
		env = cpy.env;
		path = cpy.path;
		request = cpy.request;
		response = cpy.response;
		serverctx = cpy.serverctx;
		locationctx = cpy.locationctx;
	}
	return (*this);
}

std::string CGI::getBinPath(void)
{
	size_t	pos;

	pos = script.find_last_of(".");
	if (pos == std::string::npos)
		return ("");
	return (locationctx.getCGI()[script.substr(pos)]);
}

void CGI::prepareResponse(std::string &out)
{
	std::string	key;
	std::string	line;
	std::string	value;
	std::ifstream	&output = response->getIfs();
	std::stringstream	ss;
	Map	&headers = response->getHeaders();

	output.open(out);
	if (!output.good())
		throw ResponseErrorException(InternalServerError);
	while (std::getline(output, line))
	{
		if (line.empty())
			break;
		if (line.back() == '\r')
		{
			line.pop_back();
			if (line.empty())
				break;
		}
		ss.str(line);
		std::getline(ss, key, ':');
		std::getline(ss, value, ':');
		Utils::toLower(key);
		if (key != "status")
			headers[key] = value;
		else
			response->setStatusLine(HTTP_VERSION + value + CRLF);
		ss.clear();
	}
	headers["content-length"] = Utils::numberToString(Utils::getFileSize("/tmp/output") - output.tellg());
}

void CGI::execute(void)
{
	pid_t	pid;
	std::string	bin;
	std::string	out;

	out = "/tmp/output";
	bin = getBinPath();
	if (access(bin.c_str(), F_OK | X_OK))
		throw ResponseErrorException(BadGateway);
	pid = runCGIProcess(bin, out);
	if (pid < 0)
		throw ResponseErrorException(InternalServerError);
	traceCGIProcess(pid);
	prepareResponse(out);
}

void CGI::traceCGIProcess(pid_t pid)
{
	int	status;
	time_t	start;
	bool	killed = false;

	start = std::time(NULL);
	while (!waitpid(pid, &status, WNOHANG))
	{
		if (std::difftime(std::time(NULL), start) > locationctx.getCGI_timeout())
		{
			if (kill(pid, SIGKILL))
				throw ResponseErrorException(InternalServerError);
			killed = true;
		}
	}
	std::remove(request->getBodyPath().c_str());
	if (killed)
		throw ResponseErrorException(GetwayTimeout);
}

pid_t CGI::runCGIProcess(std::string &bin, std::string __unused &out)
{
	int	_in = -1337;
	int	_out = -1337;
	pid_t	pid;
	std::vector<char *>	envv;

	for (size_t i = 0; i < env.size(); i++)
		envv.push_back((char *) env[i].c_str());
	envv.push_back(NULL);
	if ((pid = fork()) < 0 || access(bin.c_str(), F_OK | X_OK) != 0)
		return (-1); // err
	if (!pid)
	{
		char *args[3] = {(char *)bin.c_str(), (char *)script.c_str(), NULL};
		if (Utils::stringEndsWith(script, ".php"))
			args[1] = NULL;
		if ((_out = open(out.c_str(), O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0)
			exit(2);
		if (!request->getBodyPath().empty() && (_in = open(request->getBodyPath().c_str(), O_RDONLY, 0644)) < 0)
		{
			close(_out);
			exit(2);
		}
		if (dup2(_out, 1) < 0)
		{
			close(_in);
			close(_out);
			exit(2);
		}
		if (_in != -1337)
		{
			if (dup2(_in, 0) < 0)
			{
				close(_in);
				close(_out);
				exit(2);
			}
			close(_in);
		}
		else
			close(0);
		close(_out);
		if (chdir(dir.c_str()) != 0)
			exit(1);
		if (execve(args[0], args, envv.data()) != 0)
			exit(1);
	}
	return (pid);
}

void CGI::setupEnv(std::string bodyPath)
{
	try
	{
		if (!request || !response)
			throw ResponseErrorException(InternalServerError);
		processFilePath(bodyPath);
		env.push_back("GATEWAY_INTERFACE=CGI/1.1");
		env.push_back("SERVER_SOFTWARE=webserv/1.0");
		env.push_back("SERVER_PORT=" + serverctx.getPort());
		env.push_back("REQUEST_METHOD=" + request->getMethod());
		env.push_back("QUERY_STRING=" + request->getQueryString());
		env.push_back("SERVER_PROTOCOL=" + request->getProtocol());
		env.push_back("PATH_INFO=" +  path.substr(0, path.find_last_of('/')));
		env.push_back("SCRIPT_FILENAME=" + script);
		env.push_back("SCRIPT_NAME=" + script);
		env.push_back("DOCUMENT_ROOT=" + dir);
		env.push_back("CONTENT_TYPE=" + request->getHeaderByName("content-type"));
		env.push_back("CONTENT_LENGTH=" + request->getHeaderByName("content-length"));
		env.push_back("REDIRECT_STATUS=" + Utils::numberToString(request->getStatus()));
		env.push_back("PATH=" + PATH);
		Map m = request->getHeaders();
		for (Map::iterator it = m.begin(); it != m.end(); it++)
		{
			std::string first = it->first;
			Utils::toLower(first);
			if (first != "content-length" && first != "content-type")
				env.push_back(Utils::envName(it->first) + "=" + it->second);
		}
	}
	catch(const std::exception& e)
	{
		throw ResponseErrorException(InternalServerError);
	}
}

void CGI::processFilePath(std::string &str)
{
	std::stringstream ss(str);
	std::string	name;

	if (str[0] != '/')
		path = '.';
	while (std::getline(ss,name, '/'))
	{
		if (name.empty())
			continue;
		if (!Utils::isDirectory(path + name))
		{
			dir = path;
			script = name;
		}
		path +=  "/" + name;
	}
}

void CGI::setPath(char **_env)
{
	std::string	var;
	for (size_t i = 0; _env && _env[i]; i++)
	{
		var = _env[i];
		if (Utils::stringStartsWith(var, "PATH="))
		{
			PATH =  var.substr(5);
			break;
		}
	}
	if (!_env || CGI::PATH.empty())
		throw Fail("needs PATH variable");
}

CGI::~CGI(void)
{
	std::remove(request->getBodyPath().c_str());
}
