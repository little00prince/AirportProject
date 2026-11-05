#pragma once
#include "Aircraft.h"
#include "EnvironmentProxy.h"
#include "Managers.h"
#include <memory>
#include <string>

class Airport {
public:
    Airport(std::string name, LandingManager landingManager,
        GateManager gateManager, ServiceManager serviceManager,
        std::shared_ptr<IEnvironment> environment);
    void HandleArrival(const Aircraft& aircraft);
private:
    std::string name_;
    LandingManager landingManager_;
    GateManager gateManager_;
    ServiceManager serviceManager_;
    std::shared_ptr<IEnvironment> environment_;
};
