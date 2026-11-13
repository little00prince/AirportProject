#include "Managers.h"
#include <iostream>

void LandingManager::ScheduleLanding(const Aircraft& aircraft) {
    std::cout << "[LandingManager] Назначена ВПП для рейса "
        << aircraft.GetFlightNumber()
        << " (категория: " << aircraft.GetCategory() << ")\n";
}

void GateManager::AssignGate(const Aircraft& aircraft) {
    std::cout << "[GateManager] Назначен свободный гейт рейсу "
        << aircraft.GetFlightNumber() << "\n";
}

ServiceManager::ServiceManager() = default;

ServiceManager::ServiceManager(std::shared_ptr<IService> service)
    : service_(std::move(service)) {}

void ServiceManager::SetService(std::shared_ptr<IService> service) {
    service_ = std::move(service);
}

void ServiceManager::RequestServices(const Aircraft& aircraft) {
    std::cout << "[ServiceManager] Запрошено наземное обслуживание рейса "
        << aircraft.GetFlightNumber() << "\n";
    if (service_) {
        service_->Serve(aircraft);
    }
    else {
        std::cout << "[ServiceManager] Пакет служб не настроен\n";
    }
}
