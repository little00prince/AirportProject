#include "Aircraft.h"

Aircraft::Aircraft(std::string flightNumber, std::string type)
    : flightNumber_(std::move(flightNumber)), type_(std::move(type)) {}

const std::string& Aircraft::GetFlightNumber() const { return flightNumber_; }
const std::string& Aircraft::GetType() const { return type_; }

PassengerLiner::PassengerLiner(const std::string& flightNumber)
    : Aircraft(flightNumber, "пассажирский лайнер") {}
std::string PassengerLiner::GetCategory() const { return "пассажирский"; }

EmergencyAircraft::EmergencyAircraft(const std::string& flightNumber)
    : Aircraft(flightNumber, "аварийное воздушное судно") {}
std::string EmergencyAircraft::GetCategory() const { return "аварийный"; }
