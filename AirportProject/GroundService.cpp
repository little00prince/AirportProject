#include "GroundService.h"
#include <iostream>

void FuelService::Refuel(const Aircraft& aircraft) {
    std::cout << "[Заправка] Заправка воздушного судна " << aircraft.GetFlightNumber()
        << " (" << aircraft.GetType() << ")\n";
}

void BaggageService::UnloadBaggage(const Aircraft& aircraft) {
    std::cout << "[Багаж] Выгрузка багажа с рейса " << aircraft.GetFlightNumber() << "\n";
}

void BaggageService::LoadBaggage(const Aircraft& aircraft) {
    std::cout << "[Багаж] Погрузка багажа на рейс " << aircraft.GetFlightNumber() << "\n";
}

void CleaningService::CleanCabin(const Aircraft& aircraft) {
    std::cout << "[Уборка] Уборка салона воздушного судна " << aircraft.GetFlightNumber() << "\n";
}

void CateringService::ReplenishSupplies(const Aircraft& aircraft) {
    std::cout << "[Питание] Пополнение бортового питания рейса " << aircraft.GetFlightNumber() << "\n";
}

GroundService::GroundService() {
}

void GroundService::ServeAircraft(const Aircraft& aircraft) {
    std::cout << "[НаземнаяСлужба] Начало обслуживания рейса " << aircraft.GetFlightNumber() << "\n";

    baggageService_.UnloadBaggage(aircraft);
    cleaningService_.CleanCabin(aircraft);
    fuelService_.Refuel(aircraft);
    cateringService_.ReplenishSupplies(aircraft);
    baggageService_.LoadBaggage(aircraft);

    std::cout << "[НаземнаяСлужба] Обслуживание рейса " << aircraft.GetFlightNumber() << " завершено\n";
}
