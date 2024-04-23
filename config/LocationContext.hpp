/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 11:30:59 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/09 14:02:11 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Context.hpp"

class LocationContext : public Context
{
    private:
        std::string prefix;
    public:
        LocationContext();
        LocationContext(const std::string& prefix);
        LocationContext(const LocationContext& obj);
        LocationContext& operator=(const LocationContext& obj);
        ~LocationContext();

        const std::string& getPrefix() const;
};