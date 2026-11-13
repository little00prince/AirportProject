#include "AircraftFactory.h"
#include <iostream>

std::shared_ptr<Aircraft> AircraftCreator::RegisterFlight(
    const std::string& flightNumber)
{
    std::cout << "[AircraftCreator] RegisterFlight: запрос на создание борта "
        << flightNumber << "\n";
    // Фабричный метод — решение о конкретном классе у подкласса
    std::shared_ptr<Aircraft> aircraft = CreateAircraft(flightNumber);
    std::cout << "[AircraftCreator] Создан борт типа \""
        << aircraft->GetType() << "\", категория: "
        << aircraft->GetCategory() << "\n";
    return aircraft;
}

std::shared_ptr<Aircraft> PassengerAircraftCreator::CreateAircraft(
    const std::string& flightNumber)
{
    std::cout << "[PassengerAircraftCreator] Factory Method -> PassengerLiner\n";
    return std::shared_ptr<Aircraft>(new PassengerLiner(flightNumber));
}

std::shared_ptr<Aircraft> EmergencyAircraftCreator::CreateAircraft(
    const std::string& flightNumber)
{
    std::cout << "[EmergencyAircraftCreator] Factory Method -> EmergencyAircraft\n";
    return std::shared_ptr<Aircraft>(new EmergencyAircraft(flightNumber));
}
