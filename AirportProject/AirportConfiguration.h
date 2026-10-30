#pragma once
#include <string>

struct AirportConfiguration {
    std::string airportName;
    bool useProxy = true;
    bool cacheEnabled = true;
    bool weatherAccessAllowed = true;
};
