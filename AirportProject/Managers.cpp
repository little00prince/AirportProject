#include "Managers.h"
#include <iostream>

void LandingManager::ScheduleLanding(const Aircraft& aircraft) {
    std::cout << "[LandingManager] Назначена ВПП для рейса "
        << aircraft.GetFlightNumber()
        << " (категория: " << aircraft.GetCategory() << ")\n";
}

GateManager::GateManager()
    : pool_(), currentGate_() {}

GateManager::GateManager(std::shared_ptr<GatePool> pool)
    : pool_(std::move(pool)), currentGate_() {}

void GateManager::AssignGate(const Aircraft& aircraft) {
    if (!pool_) {
        std::cout << "[GateManager] Пул гейтов не задан, назначение пропущено\n";
        return;
    }
    currentGate_ = pool_->Acquire(aircraft.GetFlightNumber());
    if (currentGate_) {
        std::cout << "[GateManager] Рейсу " << aircraft.GetFlightNumber()
            << " назначен гейт " << currentGate_->Number() << "\n";
    }
    else {
        std::cout << "[GateManager] Не удалось назначить гейт рейсу "
            << aircraft.GetFlightNumber() << "\n";
    }
}

void GateManager::ReleaseAssignedGate() {
    if (pool_ && currentGate_) {
        pool_->Release(currentGate_);
        currentGate_ = std::shared_ptr<Gate>();
    }
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
