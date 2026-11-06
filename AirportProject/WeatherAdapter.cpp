#include "WeatherAdapter.h"
#include <iostream>

LegacyWeatherStation::LegacyWeatherStation(double windKmH, std::string direction,
    double visibilityMeters, double pressureHPa)
    : windKmH_(windKmH),
    direction_(std::move(direction)),
    visibilityMeters_(visibilityMeters),
    pressureHPa_(pressureHPa) {}

double LegacyWeatherStation::fetchWindKmH() const {
    return windKmH_;
}

std::string LegacyWeatherStation::fetchDirection() const {
    return direction_;
}

double LegacyWeatherStation::fetchVisibilityM() const {
    return visibilityMeters_;
}

double LegacyWeatherStation::fetchPressureHPa() const {
    return pressureHPa_;
}

WeatherAdapter::WeatherAdapter(std::shared_ptr<LegacyWeatherStation> legacy)
    : legacy_(std::move(legacy)) {}

double WeatherAdapter::GetWindSpeed() {
    // устаревшая станция возвращает км/ч, наш интерфейс ожидает м/с
    double ms = legacy_->fetchWindKmH() / 3.6;
    std::cout << "[Адаптер] Перевод ветра: "
        << legacy_->fetchWindKmH() << " км/ч -> " << ms << " м/с\n";
    return ms;
}

std::string WeatherAdapter::GetWindDirection() {
    return legacy_->fetchDirection();
}

double WeatherAdapter::GetVisibility() {
    // устаревшая станция возвращает метры, наш интерфейс ожидает км
    double km = legacy_->fetchVisibilityM() / 1000.0;
    std::cout << "[Адаптер] Перевод видимости: "
        << legacy_->fetchVisibilityM() << " м -> " << km << " км\n";
    return km;
}

double WeatherAdapter::GetPressure() {
    return legacy_->fetchPressureHPa();
}
