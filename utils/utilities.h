#ifndef     _UTILITIES_H_
#define     _UTILITIES_H_

#include <string>
#include <vector>


/* takes the string returned from Wt::Http::Request::pathInfo()
 * and returns a vector containing the resouce identifiers without
 * the '/' 
 */
std::vector<std::string>    analyzeUrl(const std::string&   url)
{
    std::vector<std::string>    result;
    std::string::size_type      pos = 0;
    std::string::size_type      count;

    while(1) 
    {
        pos = url.find('/',pos)+1;
        count = url.find('/', pos);

        if( count == std::basic_string<char>::npos) 
        {
            result.push_back(url.substr(pos, url.size()));
            break;
        }
        else   
            result.push_back(url.substr(pos, count-pos));
    }

    return result;
}


#endif
