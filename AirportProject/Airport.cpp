#include "Airport.h"
#include <iostream>

Airport::Airport(std::string name, LandingManager landingManager,
    GateManager gateManager, ServiceManager serviceManager,
    std::shared_ptr<IEnvironment> environment)
    : name_(std::move(name)), landingManager_(landingManager),
    gateManager_(gateManager), serviceManager_(serviceManager),
    environment_(std::move(environment)) {}

void Airport::HandleArrival(const Aircraft& aircraft) {
    std::cout << "\n[Аэропорт] " << name_ << ": обработка прибытия рейса "
        << aircraft.GetFlightNumber() << "\n";

    // Сначала запрашиваем данные, чтобы сообщения Proxy не разрывали строку погоды
    double wind = environment_->GetWindSpeed();
    std::string direction = environment_->GetWindDirection();
    double visibility = environment_->GetVisibility();
    double pressure = environment_->GetPressure();

    std::cout << "[Аэропорт] Погодные условия: ветер "
        << wind << " м/с, направление "
        << direction << ", видимость "
        << visibility << " км, давление "
        << pressure << " гПа\n";
    landingManager_.ScheduleLanding(aircraft);
    gateManager_.AssignGate(aircraft);
    serviceManager_.RequestServices(aircraft);
    std::cout << "[Аэропорт] Обработка прибытия завершена\n";
}
