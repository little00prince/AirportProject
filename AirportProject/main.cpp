#include "AirportFacade.h"
#include "AirportConfiguration.h"
#include "Aircraft.h"
#include "AircraftType.h"
#include "AircraftFactory.h"
#include <iostream>
#include <memory>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    AirportConfiguration config;
    config.airportName = "Международный аэропорт";
    config.useProxy = true;
    config.cacheEnabled = true;
    config.weatherAccessAllowed = true;
    config.useAdapter = false;
    config.useLogging = true;
    config.useTiming = true;
    config.usePriority = true;

    std::cout << "========================================\n";
    std::cout << "  State + Observer (+ Factory Method, Object Pool, ...)\n";
    std::cout << "========================================\n\n";

    AirportFacade facade(config);

    // ---------- Factory Method: создаём борта через создателей ----------
    std::cout << "----------------------------------------\n";
    std::cout << "  ПАТТЕРН: Factory Method\n";
    std::cout << "----------------------------------------\n";
    PassengerAircraftCreator passengerCreator;
    EmergencyAircraftCreator emergencyCreator;

    std::shared_ptr<Aircraft> flight1 = passengerCreator.RegisterFlight("SU-123");
    std::shared_ptr<Aircraft> flight2 = passengerCreator.RegisterFlight("SU-456");
    std::shared_ptr<Aircraft> emergency1 = emergencyCreator.RegisterFlight("MED-01");
    std::shared_ptr<Aircraft> emergency2 = emergencyCreator.RegisterFlight("MED-02");

    std::cout << "[AircraftTypeFactory] Размер пула типов (Flyweight): "
        << AircraftTypeFactory::PoolSize() << "\n\n";

    // ---------- Прибытия: Object Pool гейтов внутри Facade/GateManager ----------
    std::cout << "----------------------------------------\n";
    std::cout << "  Пассажирский рейс (State + Observer + Object Pool)\n";
    std::cout << "----------------------------------------\n";
    facade.ProcessArrival(*flight1);

    std::cout << "\n----------------------------------------\n";
    std::cout << "  Аварийный рейс\n";
    std::cout << "----------------------------------------\n";
    facade.ProcessArrival(*emergency1);

    std::cout << "\n----------------------------------------\n";
    std::cout << "  Второй пассажирский (гейты снова из пула)\n";
    std::cout << "----------------------------------------\n";
    facade.ProcessArrival(*flight2);

    std::cout << "\n----------------------------------------\n";
    std::cout << "  Object Pool: нехватка гейтов\n";
    std::cout << "----------------------------------------\n";
    facade.DemonstrateNoFreeGates();

    std::cout << "\n=== Демонстрация завершена ===\n";
    return 0;
}
