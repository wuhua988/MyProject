#include "ExtendString.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>

void ExtendString::lower(std::string &s)
{
    for(unsigned int i=0; i<s.size(); i++)
    {
        if(s[i] >= 'A' && s[i] <= 'Z')
            s[i] += ('a'-'A');
    }
}

void ExtendString::upper(std::string &s)
{
    for(unsigned int i=0; i<s.size(); i++)
    {
        if(s[i] >= 'a' && s[i] <= 'z')
            s[i] -= ('a'-'A');
    }
}

void ExtendString::lstrip(std::string &s)
{
    std::string whitespaces (" \t\f\v\n\r");
    size_t pos = s.find_first_not_of(whitespaces);
    if( pos != std::string::npos ) {
        s.erase(0, pos);
    } else {
        s.clear();
    }
}

void ExtendString::rstrip(std::string &s)
{
    std::string whitespaces (" \t\f\v\n\r");
    size_t pos = s.find_last_not_of(whitespaces);
    if( pos != std::string::npos ) {
        s.erase(pos + 1);        
    } else {
        s.clear();
    }
}

void ExtendString::strip(std::string &s)
{
   lstrip(s);
   rstrip(s);
}

void ExtendString::split(const std::string &s, 
           std::vector<std::string>& v, 
           char delim)
{
    std::stringstream ss(s);
    std::string item;
    v.clear();
    while (std::getline(ss, item, delim)) {
        v.push_back(item);
    }
}

bool ExtendString::startswith(const std::string& s, const std::string subs)
{
   return s.find(subs) == 0;
}

bool ExtendString::endswith(const std::string& s, const std::string subs)
{
   size_t pos = s.rfind(subs);
   return (pos != std::string::npos) &&
          (pos == (s.length() - subs.length()));
}    

bool ExtendString::iequals(const std::string& a, const std::string& b)
{
    size_t sz = a.length();
    if (b.length() != sz)
        return false;
    for (size_t i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}