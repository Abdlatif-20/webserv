/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 16:38:05 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/30 14:24:12 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include "Request.hpp"
#include "Response.hpp"
#include "ResponseUtils.hpp"

class Response;

class CGI
{
	private:
		Request		*request;
		Response	*response;

		std::string		dir;
		std::string		path;
		std::string		script;
		ServerContext	serverctx;
		LocationContext	locationctx;

		std::vector<std::string>	env;

	public:
		static std::string	PATH;

		CGI(void);
		CGI(const CGI& cpy);
		CGI(Response *_response, Request *_request);

		CGI&	operator=(const CGI& cpy);

		std::string	getBinPath(void);

		pid_t	runCGIProcess(std::string &bin, std::string &output);

		void	execute(void);
		void	traceCGIProcess(pid_t	pid);
		void	setupEnv(std::string bodyPath);
		void	processFilePath(std::string& str);

		// static member functions
		static void	setPath(char **_env);

		~CGI(void);
};

#endif
