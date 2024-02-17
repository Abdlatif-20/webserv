/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/02/17 11:40:23 by mel-yous         ###   ########.fr       */
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
    errorPages = obj.errorPages;
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

void Context::addErrorPage(StringVector error_page)
{
    errorPages.push_back(error_page);
}

const DirectivesMap& Context::getDirectives() const
{
    return directives;
}

const std::vector<StringVector>& Context::getErrorPages() const
{
    return errorPages;
}

std::string Context::getRoot() const
{
    DirectivesMap::const_iterator it = directives.find("root");
    if (it != directives.end())
        return *it->second.cbegin();
    return "assets/www";
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
    else
        indexPath = "html/index.html";
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

StringVector Context::getAllowedMethods() const
{
    DirectivesMap::const_iterator it = directives.find("allowed_methods");
    StringVector vec;
    if (it != directives.cend())
    {
        StringVector::const_iterator vec_it = it->second.cbegin();
        while (vec_it != it->second.cend())
        {
            if (*vec_it == "GET" || *vec_it == "POST" || *vec_it == "DELETE")
                vec.push_back(*vec_it);
            vec_it++;
        }
    }
    else
    {
        vec.push_back("GET");
        vec.push_back("POST");
        vec.push_back("DELETE");
    }
    return vec;
}

std::string Context::getUploadStore() const
{
    DirectivesMap::const_iterator it = directives.find("upload_store");
    if (it != directives.cend())
        return *it->second.cbegin();
    return "assets/upload";
}

std::string Context::getErrorPage(const std::string& status) const
{
    std::vector<StringVector>::const_iterator it = errorPages.cbegin();
    StringVector vec;
    while (it != errorPages.cend())
    {
        vec = *it;
        StringVector::const_iterator vec_it = std::find(vec.cbegin(), vec.cend(), status);
        if (vec_it != vec.cend())
        {
            while (vec_it != (vec.cend() - 1))
                vec_it++;
            return *vec_it;
        }
        it++;
    }
    return "";
}