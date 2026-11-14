#include "AirportFacade.h"
#include "Airport.h"
#include "Services.h"
#include "WeatherAdapter.h"
#include "EnvironmentProxy.h"
#include "FlightLifecycle.h"
#include <iostream>

AirportFacade::AirportFacade(AirportConfiguration config)
    : config_(std::move(config))
{
    // По умолчанию обычные объявления в консоль
    std::shared_ptr<IAnnouncementImplementor> impl(new ConsoleAnnouncementImplementor());
    announcement_ = std::shared_ptr<Announcement>(new DetailedAnnouncement(impl));
    // Object Pool: общий пул гейтов на все прибытия через этот фасад
    gatePool_ = std::shared_ptr<GatePool>(new GatePool(2));
}

std::shared_ptr<IEnvironment> AirportFacade::BuildWeather(bool forceAdapter) const {
    if (forceAdapter || config_.useAdapter) {
        std::shared_ptr<LegacyWeatherStation> legacy(
            new LegacyWeatherStation(23.4, "северо-западное", 8000.0, 1013.2));
        std::shared_ptr<IEnvironment> adapter(new WeatherAdapter(legacy));
        std::cout << "[AirportFacade] WeatherAdapter: LegacyWeatherStation -> IEnvironment\n";
        return adapter;
    }

    std::shared_ptr<Environment> realEnvironment(
        new Environment(6.5, "северо-западное", 8.0, 1013.2));

    if (config_.useProxy) {
        std::shared_ptr<EnvironmentProxy> proxy(new EnvironmentProxy(realEnvironment));
        proxy->SetAccessAllowed(config_.weatherAccessAllowed);
        proxy->SetCacheEnabled(config_.cacheEnabled);
        std::cout << "[AirportFacade] EnvironmentProxy"
            << (config_.cacheEnabled ? " (кэш вкл.)" : "")
            << "\n";
        return proxy;
    }

    std::cout << "[AirportFacade] Environment (напрямую)\n";
    return realEnvironment;
}

// Пакет собирается по Information Expert (методы Needs* у Aircraft)
static std::shared_ptr<IService> BuildPackageFromExpert(
    const Aircraft& aircraft,
    const AirportConfiguration& config)
{
    std::shared_ptr<CompositeService> package(
        new CompositeService(aircraft.GetCategory() == "аварийный"
            ? "CompositeService_Emergency"
            : "CompositeService_Standard"));

    std::cout << "[InformationExpert] Запрос потребностей рейса "
        << aircraft.GetFlightNumber()
        << " (категория: " << aircraft.GetCategory() << ")\n";

    if (aircraft.NeedsMedical()) {
        package->AddService(std::shared_ptr<IService>(new MedicalService()));
        std::cout << "[InformationExpert] + MedicalService\n";
    }
    if (aircraft.NeedsBaggage()) {
        package->AddService(std::shared_ptr<IService>(new BaggageService()));
        std::cout << "[InformationExpert] + BaggageService\n";
    }
    if (aircraft.NeedsCleaning()) {
        package->AddService(std::shared_ptr<IService>(new CleaningService()));
        std::cout << "[InformationExpert] + CleaningService\n";
    }
    if (aircraft.NeedsFuel()) {
        package->AddService(std::shared_ptr<IService>(new FuelService()));
        std::cout << "[InformationExpert] + FuelService\n";
    }
    if (aircraft.NeedsCatering()) {
        package->AddService(std::shared_ptr<IService>(new CateringService()));
        std::cout << "[InformationExpert] + CateringService\n";
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

void AirportFacade::ProcessArrival(const Aircraft& aircraft) {
    std::cout << "[AirportFacade] ProcessArrival: упрощённый вход в подсистему\n";

    // State + Observer: жизненный цикл рейса и уведомления наблюдателей
    {
        FlightLifecycle lifecycle(aircraft);
        lifecycle.Attach(std::shared_ptr<IFlightObserver>(new ConsoleBoardObserver()));
        lifecycle.Attach(std::shared_ptr<IFlightObserver>(new GroundOpsObserver()));
        lifecycle.RunToCompletion();
        std::cout << "[AirportFacade] Жизненный цикл: "
            << lifecycle.CurrentStateName() << "\n";
    }

    // Bridge: для аварийных — другая реализация объявлений
    if (aircraft.GetCategory() == "аварийный") {
        std::shared_ptr<IAnnouncementImplementor> priorityImpl(
            new PriorityAnnouncementImplementor());
        announcement_->SetImplementor(priorityImpl);
    }
    else {
        std::shared_ptr<IAnnouncementImplementor> consoleImpl(
            new ConsoleAnnouncementImplementor());
        announcement_->SetImplementor(consoleImpl);
    }
    announcement_->AnnounceArrival(aircraft);

    // Аварийным — погода через Adapter, остальным — по config (обычно Proxy)
    bool forceAdapter = (aircraft.GetCategory() == "аварийный");
    std::shared_ptr<IEnvironment> weather = BuildWeather(forceAdapter);

    std::shared_ptr<IService> package = BuildPackageFromExpert(aircraft, config_);
    ServiceManager serviceManager(package);

    GateManager gateManager(gatePool_);
    Airport airport(
        config_.airportName,
        LandingManager{},
        gateManager,
        serviceManager,
        weather
    );
    airport.HandleArrival(aircraft);

    std::cout << "[AirportFacade] ProcessArrival завершён\n";
}

void AirportFacade::DemonstrateNoFreeGates() {
    std::cout << "[AirportFacade] Демо Object Pool: занимаем все гейты\n";
    std::shared_ptr<Gate> held1 = gatePool_->Acquire("HOLD-1");
    std::shared_ptr<Gate> held2 = gatePool_->Acquire("HOLD-2");

    std::cout << "[AirportFacade] Пытаемся выдать гейт ещё одному рейсу...\n";
    std::shared_ptr<Gate> overflow = gatePool_->Acquire("OVERFLOW-99");
    if (!overflow) {
        std::cout << "[AirportFacade] Ожидаемо: свободных гейтов нет "
            "(пул исчерпан)\n";
    }

    std::cout << "[AirportFacade] Освобождаем занятые для демо гейты\n";
    gatePool_->Release(held1);
    gatePool_->Release(held2);
}
