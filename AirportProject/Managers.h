#pragma once
#include "Aircraft.h"
#include "Services.h"
#include "GatePool.h"
#include <memory>

class LandingManager {
public:
    void ScheduleLanding(const Aircraft& aircraft);
};

class GateManager {
public:
    GateManager();
    explicit GateManager(std::shared_ptr<GatePool> pool);

    void AssignGate(const Aircraft& aircraft);
    void ReleaseAssignedGate();

private:
    std::shared_ptr<GatePool> pool_;
    std::shared_ptr<Gate> currentGate_;
};

class ServiceManager {
public:
    ServiceManager();
    explicit ServiceManager(std::shared_ptr<IService> service);
    void RequestServices(const Aircraft& aircraft);
    void SetService(std::shared_ptr<IService> service);

private:
    std::shared_ptr<IService> service_;
};
