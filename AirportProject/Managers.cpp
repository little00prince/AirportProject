#include "Managers.h"
#include <iostream>

void LandingManager::ScheduleLanding(const Aircraft& aircraft) {
    std::cout << "[LandingManager] Подобрана ВПП для рейса "
        << aircraft.GetFlightNumber()
        << " (категория: " << aircraft.GetCategory() << ")\n";
}

void GateManager::AssignGate(const Aircraft& aircraft) {
    std::cout << "[GateManager] Назначен свободный гейт рейсу "
        << aircraft.GetFlightNumber() << "\n";
}

ServiceManager::ServiceManager() {
}

ServiceManager::ServiceManager(GroundService groundService)
    : groundService_(groundService) {
}

void ServiceManager::RequestServices(const Aircraft& aircraft) {
    std::cout << "[ServiceManager] Запрошено наземное обслуживание рейса "
        << aircraft.GetFlightNumber() << "\n";
    groundService_.ServeAircraft(aircraft);
}
