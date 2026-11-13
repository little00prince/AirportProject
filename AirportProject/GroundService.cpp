#include "GroundService.h"
#include <iostream>

void FuelService::Refuel(const Aircraft& aircraft) {
    std::cout << "[FuelService] Заправка самолета " << aircraft.GetFlightNumber()
        << " (" << aircraft.GetType() << ")\n";
}

void BaggageService::UnloadBaggage(const Aircraft& aircraft) {
    std::cout << "[BaggageService] Выгрузка багажа из самолета " << aircraft.GetFlightNumber() << "\n";
}

void BaggageService::LoadBaggage(const Aircraft& aircraft) {
    std::cout << "[BaggageService] Погрузка багажа в самолет " << aircraft.GetFlightNumber() << "\n";
}

void CleaningService::CleanCabin(const Aircraft& aircraft) {
    std::cout << "[CleaningService] Клининг кабины самолета " << aircraft.GetFlightNumber() << "\n";
}

void CateringService::ReplenishSupplies(const Aircraft& aircraft) {
    std::cout << "[CateringService] Пополнение бортового питания на время полета " << aircraft.GetFlightNumber() << "\n";
}

void GroundService::ServeAircraft(const Aircraft& aircraft) {
    std::cout << "[GroundService] Старт наземных служб аэропорта "
        << aircraft.GetFlightNumber() << "\n";

    if (package_.baggage)  baggageService_.UnloadBaggage(aircraft);
    if (package_.cleaning) cleaningService_.CleanCabin(aircraft);
    if (package_.fuel)     fuelService_.Refuel(aircraft);
    if (package_.catering) cateringService_.ReplenishSupplies(aircraft);
    if (package_.baggage)  baggageService_.LoadBaggage(aircraft);

    std::cout << "[GroundService] Наземное обслуживание самолета "
        << aircraft.GetFlightNumber() << " Готово\n";
}