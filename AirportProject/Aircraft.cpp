#include "Aircraft.h"
#include <iostream>

Aircraft::Aircraft(std::string flightNumber, std::shared_ptr<AircraftType> type)
    : flightNumber_(std::move(flightNumber)), aircraftType_(std::move(type)) {}

const std::string& Aircraft::GetFlightNumber() const {
    return flightNumber_;
}

const std::string& Aircraft::GetType() const {
    return aircraftType_->GetTypeName();
}

std::string Aircraft::GetCategory() const {
    return aircraftType_->GetCategory();
}

bool Aircraft::NeedsBaggage() const { return true; }
bool Aircraft::NeedsCleaning() const { return true; }
bool Aircraft::NeedsFuel() const { return true; }
bool Aircraft::NeedsCatering() const { return true; }
bool Aircraft::NeedsMedical() const { return false; }

std::shared_ptr<AircraftType> Aircraft::GetAircraftType() const {
    return aircraftType_;
}

PassengerLiner::PassengerLiner(const std::string& flightNumber)
    : Aircraft(
        flightNumber,
        AircraftTypeFactory::GetType("пассажирский лайнер", "пассажирский"))
{
}

EmergencyAircraft::EmergencyAircraft(const std::string& flightNumber)
    : Aircraft(
        flightNumber,
        AircraftTypeFactory::GetType("аварийное воздушное судно", "аварийный"))
{
}

bool EmergencyAircraft::NeedsBaggage() const { return false; }
bool EmergencyAircraft::NeedsCatering() const { return false; }
bool EmergencyAircraft::NeedsMedical() const { return true; }
