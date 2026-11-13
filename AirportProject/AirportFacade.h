#pragma once
#include "Aircraft.h"
#include "AirportConfiguration.h"
#include "Announcement.h"
#include "GatePool.h"
#include <memory>
#include <string>

// Facade: упрощённый вход в подсистему прибытия
class AirportFacade {
public:
    explicit AirportFacade(AirportConfiguration config);

    // Одна операция вместо ручной сборки managers / weather / package / Airport
    void ProcessArrival(const Aircraft& aircraft);

    // Object Pool: показать отказ, когда все гейты заняты
    void DemonstrateNoFreeGates();

private:
    AirportConfiguration config_;
    std::shared_ptr<Announcement> announcement_;
    std::shared_ptr<GatePool> gatePool_;

    std::shared_ptr<class IEnvironment> BuildWeather(bool forceAdapter) const;
};
