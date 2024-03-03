/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vec.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:44:17 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/03 20:42:15 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef VEC_HPP
#define VEC_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <sys/poll.h>

// template <typename T>
class Vec
{
	private:
		// size_t	capacity;
		std::vector<pollfd>	arr;
		std::stack<size_t>	freeIndexs;
	public:
		Vec(void);
		Vec(const Vec& cpy);

		void	push_back(pollfd t);
		void	erase(size_t idx);

		size_t	size(void);
		pollfd*	data(void);

		pollfd	&operator[](size_t idx);
		Vec&	operator=(const Vec& cpy);

		~Vec(void);
};

#endif
