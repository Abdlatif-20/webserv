/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-yous <mel-yous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:24:37 by mel-yous          #+#    #+#             */
/*   Updated: 2024/03/20 02:16:34 by mel-yous         ###   ########.fr       */
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

/* `addDirective(Directive _directive)` is a member function of the `Context` class.
It takes a `Directive` object as a parameter and adds it to the `directives` member variable of
the `Context` class. The `directives` member variable is a `DirectivesMap`, which is a map where the
key is a string and the value is a vector of strings. */
void Context::addDirective(Directive _directive)
{
    directives.insert(_directive);
}

/* The `appendDirective(Directive _directive)` function in the `Context` class is used to
append directives to an existing directive in the `directives` member variable. */
void Context::appendDirective(Directive _directive)
{
    DirectivesMap::iterator it = directives.find(_directive.first);
    it->second.insert(it->second.end(), _directive.second.begin(), _directive.second.end());
}

/* The `addErrorPage` function in the `Context` class is used to
add an error page to the `errorPages` member variable. It takes a `StringVector` containing the
error page information as a parameter and adds it to the `errorPages` vector in the `Context` class.
The `errorPages` vector stores multiple error pages, each represented as a vector of strings. */
void Context::addErrorPage(StringVector error_page)
{
    errorPages.push_back(error_page);
}

/* The `getDirectives()` function in the `Context` class is a const
member function that returns a constant reference to the `directives` member variable of the
`Context` class.*/
const DirectivesMap& Context::getDirectives() const
{
    return directives;
}

/* The `getErrorPages()` function in the `Context`
class is a const member function that returns a constant reference to the `errorPages` member
variable of the `Context` class.*/
const std::vector<StringVector>& Context::getErrorPages() const
{
    return errorPages;
}

/* The `getRoot()` function in the `Context` class is a const member
function that retrieves the value associated with the "root" directive from the `directives` member
variable. It first looks for the "root" directive in the `directives` map and returns the value
associated with it. If the "root" directive is not found, it returns a default value of
"assets/www". This function is used to get the root directory path specified in the configuration
for the server. */
std::string Context::getRoot() const
{
    DirectivesMap::const_iterator it = directives.find("root");
    if (it != directives.end())
        return *it->second.cbegin() + "/";
    return "assets/www/";
}

/* The `std::string Context::getIndex() const` function in the `Context` class is a const member
function that retrieves the index file path specified in the configuration for the server.*/
std::string Context::getIndex(const std::string& path) const
{
    DirectivesMap::const_iterator it = directives.find("index");
    std::string indexPath = "";
    StringVector::const_iterator vec_iter;

    if (it != directives.cend())
    {
        vec_iter = it->second.cbegin();
        while (vec_iter != it->second.cend())
        {
            indexPath = path + (*vec_iter);
            if (Utils::checkIfPathExists(indexPath) && Utils::isReadableFile(indexPath))
                return (*vec_iter);
            vec_iter++;
        }
        if (!Utils::checkIfPathExists(indexPath))
            throw Utils::FileNotFoundException();
        if (Utils::isDirectory(indexPath) || !Utils::isReadableFile(indexPath))
            throw Utils::FilePermissionDenied();
    }
    if (getRoot() == "assets/www/")
        return "html/index.html";
    return "";
}

/* The `bool Context::getAutoIndex() const` function in the `Context` class is a const member function
that retrieves the value associated with the "auto_index" directive from the `directives` member
variable. It checks if the "auto_index" directive is present in the directives map and if its value
is set to "on". If the directive is found and set to "on", the function returns `true`, indicating
that auto-indexing is enabled. If the directive is not found or set to any other value, the function
returns `false`, indicating that auto-indexing is not enabled. */
bool Context::getAutoIndex() const
{
    DirectivesMap::const_iterator it = directives.find("auto_index");
    if (it != directives.cend() && *it->second.cbegin() == "on")
        return true;
    return false;
}

/* The `int Context::getClientMaxBodySize() const` function in the `Context` class is a const member
function that retrieves the value associated with the "client_max_body_size" directive from the
`directives` member variable. It checks if the "client_max_body_size" directive is present in the
directives map and if it is, it converts the value to an integer using `std::atoi` and returns it.
If the directive is not found, it returns a default value of 1. This function is used to get the
maximum allowed size of the client request body specified in the server configuration. */
int Context::getClientMaxBodySize() const
{
    DirectivesMap::const_iterator it = directives.find("client_max_body_size");
    if (it != directives.cend())
        return std::atoi((*it->second.cbegin()).c_str());
    return 10000000;
}

/* The `StringVector Context::getAllowedMethods() const` function in the `Context` class is a const
member function that retrieves the value associated with the "allowed_methods" directive from the
`directives` member variable. It returns a vector of strings containing the allowed HTTP methods
specified in the server configuration. */
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

/* The `std::string Context::getUploadStore() const` function in the `Context` class is a const member
function that retrieves the value associated with the "upload_store" directive from the `directives`
member variable. */
std::string Context::getUploadStore() const
{
    DirectivesMap::const_iterator it = directives.find("upload_store");
    if (it != directives.cend())
        return getRoot() + *it->second.cbegin();
    return "assets/upload";
}

/* The `std::string Context::getErrorPage(const std::string& status) const` function in the `Context`
class is a const member function that retrieves the error page associated with a specific HTTP
status code. */
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

StringVector Context::getHttpRedirection() const
{
    DirectivesMap::const_iterator it = directives.find("return");
    if (it != directives.cend())
        return it->second;
    return StringVector(0);
}