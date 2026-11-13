#include "AirportFacade.h"
#include "AirportConfiguration.h"
#include "Aircraft.h"
#include "AircraftType.h"
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
    std::cout << "  Facade + Flyweight + Bridge + Information Expert\n";
    std::cout << "========================================\n\n";

    // Facade скрывает сборку Airport / managers / weather / package
    AirportFacade facade(config);

    // ---------- Flyweight: несколько рейсов одного типа делят AircraftType ----------
    std::cout << "----------------------------------------\n";
    std::cout << "  ПАТТЕРН: Flyweight\n";
    std::cout << "----------------------------------------\n";
    PassengerLiner flight1("SU-123");
    PassengerLiner flight2("SU-456");  // тот же тип — повторное использование из пула
    EmergencyAircraft emergency1("MED-01");
    EmergencyAircraft emergency2("MED-02"); // тот же аварийный тип из пула
    std::cout << "[AircraftTypeFactory] Размер пула: "
        << AircraftTypeFactory::PoolSize() << " (ожидается 2 типа)\n\n";

    // ---------- Information Expert + Facade + Bridge + Proxy ----------
    std::cout << "----------------------------------------\n";
    std::cout << "  Пассажирский рейс (Information Expert + Facade + Bridge + Proxy)\n";
    std::cout << "----------------------------------------\n";
    facade.ProcessArrival(flight1);

    // ---------- Аварийный: другой Bridge implementor + Adapter + другой набор служб ----------
    std::cout << "\n----------------------------------------\n";
    std::cout << "  Аварийный рейс (Information Expert + Adapter + Priority Bridge)\n";
    std::cout << "----------------------------------------\n";
    facade.ProcessArrival(emergency1);

    // Ещё один пассажирский — Flyweight уже в пуле
    std::cout << "\n----------------------------------------\n";
    std::cout << "  Второй пассажирский (Flyweight уже в пуле)\n";
    std::cout << "----------------------------------------\n";
    facade.ProcessArrival(flight2);

    std::cout << "\n=== Демонстрация завершена ===\n";
    return 0;
}
