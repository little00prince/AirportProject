#include "Airport.h"
#include "AirportConfiguration.h"
#include <iostream>
#include <memory>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    AirportConfiguration config{
        "Международный аэропорт", true, true, true
    };

    std::shared_ptr<Environment> realEnvironment(
        new Environment(6.5, "северо-западное", 8.0, 1013.2)
    );

    // Прокси того же типа, что и проксируемый объект (Environment)
    std::shared_ptr<Environment> proxy(
        new EnvironmentProxy(realEnvironment)
    );

    std::cout << "=== Настройка прокси ===\n";
    proxy->SetAccessAllowed(config.weatherAccessAllowed);
    proxy->SetCacheEnabled(config.cacheEnabled);
    std::cout << "\n";

    // --- Аэропорт 1: полный пакет ---
    ServicePackage fullPackage;
    GroundService fullGroundService(fullPackage);
    ServiceManager fullServiceManager(fullGroundService);

    Airport fullAirport(
        "Аэропорт-Полный",
        LandingManager{},
        GateManager{},
        fullServiceManager,
        proxy
    );

    // --- Аэропорт 2: без питания ---
    ServicePackage noCateringPackage;
    noCateringPackage.catering = false;
    GroundService lightGroundService(noCateringPackage);
    ServiceManager lightServiceManager(lightGroundService);

    Airport lightAirport(
        "Аэропорт-БезПитания",
        LandingManager{},
        GateManager{},
        lightServiceManager,
        proxy
    );

    std::cout << "=== Рейс в аэропорту с полным пакетом ===\n";
    PassengerLiner flight1("SU-123");
    fullAirport.HandleArrival(flight1);

    std::cout << "\n=== Рейс в аэропорту без питания ===\n";
    PassengerLiner flight2("SU-456");
    lightAirport.HandleArrival(flight2);

    std::cout << "\n=== Демонстрация кэша ===\n";
    std::cout << "Повторный запрос скорости ветра:\n";
    std::cout << "Скорость ветра: " << proxy->GetWindSpeed() << " м/с\n";

    std::cout << "\n=== Демонстрация блокировки доступа ===\n";
    proxy->SetAccessAllowed(false);
    try {
        std::cout << "Попытка получить видимость при запрещённом доступе...\n";
        proxy->GetVisibility();
    }
    catch (const std::exception& ex) {
        std::cout << "[Proxy] Исключение: " << ex.what() << "\n";
    }

    std::cout << "\n=== Восстановление доступа ===\n";
    proxy->SetAccessAllowed(true);
    std::cout << "Видимость после восстановления: "
        << proxy->GetVisibility() << " км\n";

    EmergencyAircraft emergency("MED-01");
    std::cout << "\n[Demo] Экстренный борт, категория: "
        << emergency.GetCategory() << "\n";

    return 0;
}
