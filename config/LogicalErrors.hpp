/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogicalErrors.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:14:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/16 16:19:02 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Defs.hpp"
#include "ServerContext.hpp"

class LogicalErrorException : public std::exception
{
    private:
        std::string str;
    public:
        LogicalErrorException(const std::string& str);
        ~LogicalErrorException() throw();
        const char* what() const throw();
};

void checkLogicalErrors(const ServersVector& servers);