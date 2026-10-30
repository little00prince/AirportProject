#include "Environment.h"
#include <utility>

Environment::Environment(double windSpeed, std::string windDirection,
                         double visibility, double pressure)
    : windSpeed_(windSpeed), windDirection_(std::move(windDirection)),
      visibility_(visibility), pressure_(pressure) {}

double Environment::GetWindSpeed() { return windSpeed_; }
std::string Environment::GetWindDirection() { return windDirection_; }
double Environment::GetVisibility() { return visibility_; }
double Environment::GetPressure() { return pressure_; }
bool Environment::IsNightTime() const { return false; }
