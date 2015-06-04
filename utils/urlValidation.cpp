#include <iostream>

#include "urlValidators.h"


int main() {

    std::string     url;
    std::getline(std::cin, url);


    validGet(url);

}
