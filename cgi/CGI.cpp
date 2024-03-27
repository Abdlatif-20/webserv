/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 16:38:20 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/27 18:27:39 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(void)
{
}

CGI::CGI(const CGI &cpy)
{
}

CGI &CGI::operator=(const CGI &cpy)
{
	// TODO: insert return statement here
}

void Response::setupEnv(char **_env)
{
	std::string	var;
	env_ptr = _env;
	for (size_t i = 0; env_ptr && env_ptr[i]; i++)
	{
		var = env_ptr[i];
		if (Utils::stringStartsWith(var, "PATH="))
		{
			PATH =  var.substr(5);
			break;
		}
	}
	if (!env_ptr || Response::PATH.empty())
		throw Fail("needs PATH variable");
}

CGI::~CGI(void)
{
}
