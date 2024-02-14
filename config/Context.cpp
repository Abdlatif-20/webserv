/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/14 18:43:00 by mel-yous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Context.hpp"

Context::Context()
{

}

Context::Context(const Context& obj)
{
    *this = obj;
}

Context& Context::operator=(const Context& obj)
{
    if (this == &obj)
        return *this;
    directives = obj.directives;
    return *this;
}

Context::~Context()
{

}

void Context::addDirective(Directive _directive)
{
    directives.insert(_directive);
}

void Context::appendDirective(Directive _directive)
{
    DirectivesMap::iterator it = directives.find(_directive.first);
    it->second.insert(it->second.end(), _directive.second.begin(), _directive.second.end());
}

const DirectivesMap& Context::getDirectives() const
{
    return directives;
}

std::string Context::getRoot() const
{
    DirectivesMap::const_iterator it = directives.find("root");
    if (it != directives.end())
        return *it->second.cbegin();
    return "";
}

std::string Context::getIndex() const
{
    DirectivesMap::const_iterator it = directives.find("index");
    std::string root = getRoot();
    std::string indexPath = "";
    std::ifstream ifs;

    if (root[root.length() - 1] != '/')
        root += "/";
    if (it != directives.cend())
    {
        StringVector::const_iterator vec_iter = it->second.cbegin();
        while (vec_iter != it->second.cend())
        {
            indexPath = root + (*vec_iter);
            ifs.open(indexPath);
            if (ifs.good())
                return (ifs.close(), indexPath);
            ifs.close();
            vec_iter++;
        }
    }
    return indexPath;
}

bool Context::getAutoIndex() const
{
    DirectivesMap::const_iterator it = directives.find("auto_index");
    if (it != directives.cend() && *it->second.cbegin() == "on")
        return true;
    return false;
}

int Context::getClientMaxBodySize() const
{
    DirectivesMap::const_iterator it = directives.find("client_max_body_size");
    if (it != directives.cend())
        return std::atoi((*it->second.cbegin()).c_str());
    return 1;
}

std::array<std::string, 3> Context::getAllowedMethods() const
{
    DirectivesMap::const_iterator it = directives.find("allowed_methods");
    std::array<std::string, 3> arr;
    int i = 0;
    if (it != directives.cend())
    {
        StringVector::const_iterator vec_it = it->second.cbegin();
        while (vec_it != it->second.cend())
        {
            vec_it++;
        }
    }
}