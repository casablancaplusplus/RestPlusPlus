#include <iostream>

#include "utilities.h"

// the url to analyze corresponds to the string returned by Wt::Http::Request::pathInfo()

int main() {
    
    // sample string
    std::string url = "/questions/{question_id}/choice/{choice_id}";

    std::vector<std::string> result = analyzeUrl(url);

    for(std::vector<std::string>::const_iterator    it = result.begin();
                                                    it != result.end();
                                                    ++it
       )
    {
        std::cout << *it << std::endl;
    }

    return 0;

}


