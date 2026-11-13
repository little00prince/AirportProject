#pragma once
#include <string>

struct AirportConfiguration {
    std::string airportName = "Международный аэропорт";
    bool useProxy = true;
    bool cacheEnabled = true;
    bool weatherAccessAllowed = true;
    bool useAdapter = false;   // true → WeatherAdapter поверх LegacyWeatherStation
    bool useLogging = true;
    bool useTiming = true;
    bool usePriority = true;
};
