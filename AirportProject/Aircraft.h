#pragma once
#include <string>

class Aircraft {
public:
    Aircraft(std::string flightNumber, std::string type);
    virtual ~Aircraft() = default;
    const std::string& GetFlightNumber() const;
    const std::string& GetType() const;
    virtual std::string GetCategory() const = 0;
protected:
    std::string flightNumber_;
    std::string type_;
};

class PassengerLiner : public Aircraft {
public:
    explicit PassengerLiner(const std::string& flightNumber);
    std::string GetCategory() const override;
};

class EmergencyAircraft : public Aircraft {
public:
    explicit EmergencyAircraft(const std::string& flightNumber);
    std::string GetCategory() const override;
};
