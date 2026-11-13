#pragma once
#include "Aircraft.h"
#include <memory>
#include <string>

// Factory Method: создатель сет фабричный метод CreateAircraft
class AircraftCreator {
public:
    virtual ~AircraftCreator() = default;

    // Операция-шаблон: использует фабричный метод
    std::shared_ptr<Aircraft> RegisterFlight(const std::string& flightNumber);

protected:
    // Фабричный метод — переопределяется в подклассах
    virtual std::shared_ptr<Aircraft> CreateAircraft(
        const std::string& flightNumber) = 0;
};

// Конкретный создатель пассажирских рейсов
class PassengerAircraftCreator : public AircraftCreator {
protected:
    std::shared_ptr<Aircraft> CreateAircraft(
        const std::string& flightNumber) override;
};

// Конкретный создатель аварийных бортов
class EmergencyAircraftCreator : public AircraftCreator {
protected:
    std::shared_ptr<Aircraft> CreateAircraft(
        const std::string& flightNumber) override;
};
