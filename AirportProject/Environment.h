#pragma once
#include <string>

class IEnvironment {
public:
    virtual ~IEnvironment() = default;
    virtual double GetWindSpeed() = 0;
    virtual std::string GetWindDirection() = 0;
    virtual double GetVisibility() = 0;
    virtual double GetPressure() = 0;
};

class Environment : public IEnvironment {
public:
    Environment(double windSpeed, std::string windDirection,
                double visibility, double pressure);
    double GetWindSpeed() override;
    std::string GetWindDirection() override;
    double GetVisibility() override;
    double GetPressure() override;
    bool IsNightTime() const;
private:
    double windSpeed_;
    std::string windDirection_;
    double visibility_;
    double pressure_;
};
