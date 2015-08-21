#ifndef __ExtendString_h__
#define __ExtendString_h__
#include <string>
#include <vector>

class ExtendString
{
public:
    static void lower(std::string &s);
     
    static void upper(std::string &s);
     
    static void lstrip(std::string &s);
     
    static void rstrip(std::string &s);
     
    static void strip(std::string &s);
     
    static void split(const std::string &s, 
                std::vector<std::string>& v, 
                char delim = ' ');
     
     
    static bool startswith(const std::string& s, const std::string subs);
     
    static bool endswith(const std::string& s, const std::string subs);  
     
    static bool iequals(const std::string& a, const std::string& b);
};

#endif //__ExtendString_h__