#include "GroundService.h"
#include <iostream>

void FuelService::Refuel(const Aircraft& aircraft) {
    std::cout << "[FuelService] Refueling aircraft " << aircraft.GetFlightNumber()
        << " (" << aircraft.GetType() << ")\n";
}

void BaggageService::UnloadBaggage(const Aircraft& aircraft) {
    std::cout << "[BaggageService] Unloading baggage from flight " << aircraft.GetFlightNumber() << "\n";
}

void BaggageService::LoadBaggage(const Aircraft& aircraft) {
    std::cout << "[BaggageService] Loading baggage onto flight " << aircraft.GetFlightNumber() << "\n";
}

void CleaningService::CleanCabin(const Aircraft& aircraft) {
    std::cout << "[CleaningService] Cleaning cabin of aircraft " << aircraft.GetFlightNumber() << "\n";
}

void CateringService::ReplenishSupplies(const Aircraft& aircraft) {
    std::cout << "[CateringService] Replenishing onboard catering for flight " << aircraft.GetFlightNumber() << "\n";
}

GroundService::GroundService() {
}

void GroundService::ServeAircraft(const Aircraft& aircraft) {
    std::cout << "[GroundService] Starting ground service for flight " << aircraft.GetFlightNumber() << "\n";

    baggageService_.UnloadBaggage(aircraft);
    cleaningService_.CleanCabin(aircraft);
    fuelService_.Refuel(aircraft);
    cateringService_.ReplenishSupplies(aircraft);
    baggageService_.LoadBaggage(aircraft);

    std::cout << "[GroundService] Ground service for flight " << aircraft.GetFlightNumber() << " completed\n";
}
