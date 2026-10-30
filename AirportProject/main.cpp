#include "Airport.h"
#include "AirportConfiguration.h"
#include <iostream>
#include <memory>
#include <windows.h>

int main() {
    // Включаем UTF-8 для русских сообщений в консоли
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    AirportConfiguration config{
        "Международный аэропорт", true, true, true
    };

    auto realEnvironment = std::make_shared<Environment>(
        6.5, "северо-западное", 8.0, 1013.2
    );

    std::shared_ptr<IEnvironment> weather;

    auto proxy = std::make_shared<EnvironmentProxy>(realEnvironment);

    proxy->SetAccessAllowed(config.weatherAccessAllowed);
    proxy->SetCacheEnabled(config.cacheEnabled);

    if (config.useProxy) {
        weather = proxy;
        std::cout << "[Config] Используется EnvironmentProxy, кэш включен\n\n";
    }
    else {
        weather = realEnvironment;
        std::cout << "[Config] Используется прямой Environment\n\n";
    }

    Airport airport(
        config.airportName,
        LandingManager{},
        GateManager{},
        ServiceManager{},
        weather
    );

    PassengerLiner flight("SU-123");
    airport.HandleArrival(flight);

    std::cout << "\n[Demo] Повторный запрос скорости ветра:\n";
    std::cout << "Скорость ветра: "
        << weather->GetWindSpeed() << " м/с\n";

    std::cout << "\n[Demo] Проверка ограничения доступа:\n";

    proxy->SetAccessAllowed(false);

    try {
        proxy->GetVisibility();
    }
    catch (const std::exception& ex) {
        std::cout << "[Proxy] " << ex.what() << "\n";
    }

    EmergencyAircraft emergency("MED-01");

    std::cout << "\n[Demo] Экстренный борт, категория: "
        << emergency.GetCategory() << "\n";

    return 0;
}
