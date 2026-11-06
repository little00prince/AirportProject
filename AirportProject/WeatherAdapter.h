#pragma once
#include "Environment.h"
#include <string>
#include <memory>

// ¬нешн€€ / устаревша€ метеостанци€ с несовместимым интерфейсом
class LegacyWeatherStation {
public:
    LegacyWeatherStation(double windKmH, std::string direction,
        double visibilityMeters, double pressureHPa);

    double fetchWindKmH() const;
    std::string fetchDirection() const;
    double fetchVisibilityM() const;
    double fetchPressureHPa() const;

private:
    double windKmH_;
    std::string direction_;
    double visibilityMeters_;
    double pressureHPa_;
};

// јдаптер: представл€ет LegacyWeatherStation как IEnvironment
class WeatherAdapter : public IEnvironment {
public:
    explicit WeatherAdapter(std::shared_ptr<LegacyWeatherStation> legacy);

    double GetWindSpeed() override;       // переводит км/ч ? м/с
    std::string GetWindDirection() override;
    double GetVisibility() override;      // переводит м ? км
    double GetPressure() override;

private:
    std::shared_ptr<LegacyWeatherStation> legacy_;
};
