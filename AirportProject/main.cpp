#include "Airport.h"
#include "AirportConfiguration.h"
#include "Services.h"
#include "WeatherAdapter.h"
#include <iostream>
#include <memory>
#include <windows.h>

// Собирает пакет служб под категорию воздушного судна
static std::shared_ptr<IService> BuildPackageForAircraft(
    const Aircraft& aircraft,
    const AirportConfiguration& config)
{
    std::shared_ptr<CompositeService> package;

    if (aircraft.GetCategory() == "аварийный") {
        // Аварийный: MedicalService + Cleaning + Fuel; без Baggage и Catering
        package = std::shared_ptr<CompositeService>(
            new CompositeService("CompositeService_Emergency"));
        package->AddService(std::shared_ptr<IService>(new MedicalService()));
        package->AddService(std::shared_ptr<IService>(new CleaningService()));
        package->AddService(std::shared_ptr<IService>(new FuelService()));
        std::cout << "[ServicePackage] По категории \"" << aircraft.GetCategory()
            << "\" выбран аварийный пакет: MedicalService, CleaningService, FuelService "
            "(без BaggageService, без CateringService)\n";
    }
    else {
        // Обычный пассажирский пакет
        package = std::shared_ptr<CompositeService>(
            new CompositeService("CompositeService_Standard"));
        package->AddService(std::shared_ptr<IService>(new BaggageService()));
        package->AddService(std::shared_ptr<IService>(new CleaningService()));
        package->AddService(std::shared_ptr<IService>(new FuelService()));
        package->AddService(std::shared_ptr<IService>(new CateringService()));
        std::cout << "[ServicePackage] По категории \"" << aircraft.GetCategory()
            << "\" выбран стандартный пакет: BaggageService, CleaningService, "
            "FuelService, CateringService\n";
    }

    std::shared_ptr<IService> service = package;

    if (config.usePriority) {
        service = std::shared_ptr<IService>(new PriorityServiceDecorator(service));
    }
    if (config.useTiming) {
        service = std::shared_ptr<IService>(new TimingServiceDecorator(service));
    }
    if (config.useLogging) {
        service = std::shared_ptr<IService>(new LoggingServiceDecorator(service));
    }

    return service;
}

static std::shared_ptr<IEnvironment> BuildWeather(const AirportConfiguration& config) {
    if (config.useAdapter) {
        // Только здесь (сборка) знаем про LegacyWeatherStation.
        // Environment и Airport о нём не знают — только про IEnvironment.
        std::shared_ptr<LegacyWeatherStation> legacy(
            new LegacyWeatherStation(23.4, "северо-западное", 8000.0, 1013.2));
        std::shared_ptr<IEnvironment> adapter(new WeatherAdapter(legacy));
        std::cout << "[Config] WeatherAdapter: LegacyWeatherStation -> IEnvironment\n";
        return adapter;
    }

    std::shared_ptr<Environment> realEnvironment(
        new Environment(6.5, "северо-западное", 8.0, 1013.2));

    if (config.useProxy) {
        std::shared_ptr<EnvironmentProxy> proxy(new EnvironmentProxy(realEnvironment));
        proxy->SetAccessAllowed(config.weatherAccessAllowed);
        proxy->SetCacheEnabled(config.cacheEnabled);
        std::cout << "[Config] EnvironmentProxy"
            << (config.cacheEnabled ? " (кэш вкл.)" : "")
            << "\n";
        return proxy;
    }

    std::cout << "[Config] Environment (напрямую)\n";
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
    config.useAdapter = false;
    config.useLogging = true;
    config.useTiming = true;
    config.usePriority = true;

    std::cout << "========================================\n";
    std::cout << "  КОНФИГУРАЦИЯ\n";
    std::cout << "========================================\n";

    std::shared_ptr<IEnvironment> weatherProxy = BuildWeather(config);
    std::cout << "\n";

    // ========== Пассажирский рейс: пакет по категории; обход служб через Iterator внутри Composite::Serve ==========
    std::cout << "========================================\n";
    std::cout << "  ПАТТЕРН: Composite + Decorator + Proxy\n";
    std::cout << "  (пассажирский рейс, погода через EnvironmentProxy)\n";
    std::cout << "========================================\n";
    {
        PassengerLiner flight("SU-123");

        // Пакет выбирается по категории уже существующего борта
        std::shared_ptr<IService> package = BuildPackageForAircraft(flight, config);
        ServiceManager serviceManager(package);

        Airport airport(
            config.airportName,
            LandingManager{},
            GateManager{},
            serviceManager,
            weatherProxy
        );
        airport.HandleArrival(flight);
    }

    // ========== Аварийный рейс: Adapter при прибытии + пакет по категории ==========
    std::cout << "\n========================================\n";
    std::cout << "  ПАТТЕРН: Adapter (во время прибытия)\n";
    std::cout << "  + пакет служб по типу воздушного судна\n";
    std::cout << "========================================\n";
    {
        AirportConfiguration adapterConfig = config;
        adapterConfig.useAdapter = true;
        adapterConfig.useProxy = false;

        // Погода через адаптер — Environment про Legacy не знает
        std::shared_ptr<IEnvironment> weatherAdapter = BuildWeather(adapterConfig);

        EmergencyAircraft emergency("MED-01");

        std::shared_ptr<IService> package = BuildPackageForAircraft(emergency, config);
        ServiceManager serviceManager(package);

        Airport airport(
            "Аэропорт_АварийнаяСтоянка",
            LandingManager{},
            GateManager{},
            serviceManager,
            weatherAdapter
        );
        airport.HandleArrival(emergency);
    }

    // ========== PROXY: кэш и доступ ==========
    std::cout << "\n========================================\n";
    std::cout << "  ПАТТЕРН: Proxy (кэш и доступ)\n";
    std::cout << "========================================\n";
    {
        std::shared_ptr<Environment> realEnv(
            new Environment(6.5, "северо-западное", 8.0, 1013.2));
        std::shared_ptr<EnvironmentProxy> proxy(new EnvironmentProxy(realEnv));
        proxy->SetAccessAllowed(true);
        proxy->SetCacheEnabled(true);

        std::cout << "[EnvironmentProxy] Первый GetWindSpeed (загрузка в кэш):\n";
        std::cout << "  скорость ветра = " << proxy->GetWindSpeed() << " м/с\n";
        std::cout << "[EnvironmentProxy] Повторный GetWindSpeed (из кэша):\n";
        std::cout << "  скорость ветра = " << proxy->GetWindSpeed() << " м/с\n";

        std::cout << "[EnvironmentProxy] Запрет доступа:\n";
        proxy->SetAccessAllowed(false);
        try {
            proxy->GetVisibility();
        }
        catch (const std::exception& ex) {
            std::cout << "  исключение: " << ex.what() << "\n";
        }
        proxy->SetAccessAllowed(true);
        std::cout << "[EnvironmentProxy] Доступ восстановлен, видимость = "
            << proxy->GetVisibility() << " км\n";
    }

    std::cout << "\n=== Демонстрация завершена ===\n";
    return 0;
}
