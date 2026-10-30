#include "Managers.h"

void LandingManager::ScheduleLanding(const Aircraft& aircraft) {
    std::cout << "[LandingManager] Подобрана ВПП для рейса "
              << aircraft.GetFlightNumber() << "\n";
}
void GateManager::AssignGate(const Aircraft& aircraft) {
    std::cout << "[GateManager] Назначен свободный гейт рейсу "
              << aircraft.GetFlightNumber() << "\n";
}
void ServiceManager::RequestServices(const Aircraft& aircraft) {
    std::cout << "[ServiceManager] Заказано наземное обслуживание рейса "
              << aircraft.GetFlightNumber() << "\n";
}
