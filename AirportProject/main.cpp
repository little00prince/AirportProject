#include "Airport.h"
#include "AirportConfiguration.h"
#include "Services.h"
#include "WeatherAdapter.h"
#include <iostream>
#include <memory>
#include <windows.h>

// Собираем стандартный пакет наземного обслуживания и при необходимости
// оборачиваем его декораторами
static std::shared_ptr<IService> BuildServicePackage(const AirportConfiguration& config) {
    auto package = std::make_shared<CompositeService>("СтандартныйПакет");

    // Порядок как в исходной лаб. 4: багаж, уборка, заправка, питание
    package->AddService(std::make_shared<BaggageService>());
    package->AddService(std::make_shared<CleaningService>());
    package->AddService(std::make_shared<FuelService>());
    package->AddService(std::make_shared<CateringService>());

    std::shared_ptr<IService> service = package;

    if (config.usePriority) {
        service = std::make_shared<PriorityServiceDecorator>(service);
        std::cout << "[Конфиг] Включён декоратор приоритета\n";
    }
    if (config.useTiming) {
        service = std::make_shared<TimingServiceDecorator>(service);
        std::cout << "[Конфиг] Включён декоратор замера времени\n";
    }
    if (config.useLogging) {
        service = std::make_shared<LoggingServiceDecorator>(service);
        std::cout << "[Конфиг] Включён декоратор журналирования\n";
    }

    return service;
}

static std::shared_ptr<IEnvironment> BuildWeather(const AirportConfiguration& config) {
    if (config.useAdapter) {
        auto legacy = std::make_shared<LegacyWeatherStation>(
            23.4, "северо-запад", 8000.0, 1013.2); // км/ч, м, гПа
        auto adapter = std::make_shared<WeatherAdapter>(legacy);
        std::cout << "[Конфиг] Используется WeatherAdapter (LegacyWeatherStation)\n";
        return adapter;
    }

    auto realEnvironment = std::make_shared<Environment>(
        6.5, "северо-запад", 8.0, 1013.2);

    if (config.useProxy) {
        auto proxy = std::make_shared<EnvironmentProxy>(realEnvironment);
        proxy->SetAccessAllowed(config.weatherAccessAllowed);
        proxy->SetCacheEnabled(config.cacheEnabled);
        std::cout << "[Конфиг] Используется EnvironmentProxy"
            << (config.cacheEnabled ? ", кэш включён" : "")
            << "\n";
        return proxy;
    }

    std::cout << "[Конфиг] Используется прямой Environment\n";
    return realEnvironment;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    AirportConfiguration config;
    config.airportName = "Международный аэропорт";
    config.useProxy = true;
    config.cacheEnabled = true;
    config.weatherAccessAllowed = true;
    config.useAdapter = false;   // true — для демонстрации адаптера
    config.useLogging = true;
    config.useTiming = true;
    config.usePriority = true;

    std::cout << "=== Конфигурация аэропорта ===\n";
    auto weather = BuildWeather(config);
    auto servicePackage = BuildServicePackage(config);
    std::cout << "\n";

    // Демонстрация итератора по составному пакету служб
    {
        // Для демонстрации итератора берём «чистый» пакет без декораторов
        auto plainPackage = std::make_shared<CompositeService>("ПакетДляИтератора");
        plainPackage->AddService(std::make_shared<BaggageService>());
        plainPackage->AddService(std::make_shared<CleaningService>());
        plainPackage->AddService(std::make_shared<FuelService>());
        plainPackage->AddService(std::make_shared<CateringService>());

        std::cout << "[Итератор] Службы в пакете:\n";
        auto it = plainPackage->CreateIterator();
        while (it.HasNext()) {
            auto s = it.Next();
            std::cout << "  - " << s->GetName() << "\n";
        }
        std::cout << "\n";
    }

    ServiceManager serviceManager(servicePackage);

    Airport airport(
        config.airportName,
        LandingManager{},
        GateManager{},
        serviceManager,
        weather
    );

    std::cout << "=== Прибытие: пассажирский рейс ===\n";
    PassengerLiner flight("SU-123");
    airport.HandleArrival(flight);

    std::cout << "\n=== Прибытие: аварийное воздушное судно (декоратор приоритета) ===\n";
    EmergencyAircraft emergency("MED-01");
    airport.HandleArrival(emergency);

    // Демонстрация адаптера (по желанию)
    std::cout << "\n=== Демонстрация адаптера (LegacyWeatherStation) ===\n";
    {
        AirportConfiguration adapterConfig = config;
        adapterConfig.useAdapter = true;
        adapterConfig.useProxy = false;
        auto adapterWeather = BuildWeather(adapterConfig);
        std::cout << "Скорость ветра: " << adapterWeather->GetWindSpeed() << " м/с\n";
        std::cout << "Видимость: " << adapterWeather->GetVisibility() << " км\n";
        std::cout << "Направление: " << adapterWeather->GetWindDirection() << "\n";
        std::cout << "Давление: " << adapterWeather->GetPressure() << " гПа\n";
    }

    return 0;
}
