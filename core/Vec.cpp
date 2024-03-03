/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vec.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houmanso <houmanso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:44:42 by houmanso          #+#    #+#             */
/*   Updated: 2024/03/03 20:42:40 by houmanso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Vec.hpp"

Vec::Vec(void)
{
	// nothing
}

Vec::Vec(const Vec &cpy)
{
	*this = cpy;
}

void	Vec::push_back(pollfd t)
{
	if (!freeIndexs.size())
		arr.push_back(t);
	else
	{
		arr[freeIndexs.top()] = t;
		freeIndexs.pop();
	}
}


void	Vec::erase(size_t idx)
{
	freeIndexs.push(idx);
}

pollfd	*Vec::data(void)
{
	return	(arr.data());
}


size_t Vec::size(void)
{
	return (arr.size());
}

pollfd	&Vec::operator[](size_t idx)
{
	return (arr[idx]);
}

Vec &Vec::operator=(const Vec &cpy)
{
	if (this != &cpy)
	{
		arr = cpy.arr;
		freeIndexs = cpy.freeIndexs;
	}
	return (*this);
}

Vec::~Vec(void)
{
}
