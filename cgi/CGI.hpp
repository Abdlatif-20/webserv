/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 16:38:05 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/27 16:39:41 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>

class CGI
{
	public:
		CGI(void);
		CGI(const CGI& cpy);

		CGI&	operator=(const CGI& cpy);

		static void	setupEnv(char **_env);

		~CGI(void);
};

#endif
