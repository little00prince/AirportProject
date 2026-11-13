#pragma once
#include "AircraftType.h"
#include <memory>
#include <string>

class Aircraft {
public:
    Aircraft(std::string flightNumber, std::shared_ptr<AircraftType> type);
    virtual ~Aircraft() = default;

    const std::string& GetFlightNumber() const;
    const std::string& GetType() const;
    virtual std::string GetCategory() const;

    // Information Expert: сам борт знает, какие службы ему нужны
    virtual bool NeedsBaggage() const;
    virtual bool NeedsCleaning() const;
    virtual bool NeedsFuel() const;
    virtual bool NeedsCatering() const;
    virtual bool NeedsMedical() const;

    std::shared_ptr<AircraftType> GetAircraftType() const;

protected:
    std::string flightNumber_;                 // extrinsic (уникально для рейса)
    std::shared_ptr<AircraftType> aircraftType_; // intrinsic через Flyweight
};

class PassengerLiner : public Aircraft {
public:
    explicit PassengerLiner(const std::string& flightNumber);
    // стандартные Needs* — из базового (багаж/уборка/топливо/питание)
};

class EmergencyAircraft : public Aircraft {
public:
    explicit EmergencyAircraft(const std::string& flightNumber);

    bool NeedsBaggage() const override;
    bool NeedsCatering() const override;
    bool NeedsMedical() const override;
};
