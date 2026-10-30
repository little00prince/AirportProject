#include "Airport.h"
#include <iostream>

Airport::Airport(std::string name, LandingManager landingManager,
                 GateManager gateManager, ServiceManager serviceManager,
                 std::shared_ptr<IEnvironment> environment)
    : name_(std::move(name)), landingManager_(landingManager),
      gateManager_(gateManager), serviceManager_(serviceManager),
      environment_(std::move(environment)) {}

void Airport::HandleArrival(const Aircraft& aircraft) {
    std::cout << "\n[Airport] " << name_ << ": обработка прибытия рейса "
              << aircraft.GetFlightNumber() << "\n";
    std::cout << "[Airport] Погодные условия: ветер "
              << environment_->GetWindSpeed() << " м/с, направление "
              << environment_->GetWindDirection() << ", видимость "
              << environment_->GetVisibility() << " км, давление "
              << environment_->GetPressure() << " гПа\n";
    landingManager_.ScheduleLanding(aircraft);
    gateManager_.AssignGate(aircraft);
    serviceManager_.RequestServices(aircraft);
    std::cout << "[Airport] Обработка прибытия завершена\n";
}
