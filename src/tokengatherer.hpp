#pragma once

#include <iostream>
#include <string>

class TokenGatherer {
public:
    std::string getTokenFromUser() const {
        std::string token;
        std::cout << "Enter the token symbol: ";
        std::cin >> token;
        return token;
    }
};
