#pragma once
#include "Aircraft.h"
#include "Services.h"
#include <memory>

class LandingManager {
public:
    void ScheduleLanding(const Aircraft& aircraft);
};

class GateManager {
public:
    void AssignGate(const Aircraft& aircraft);
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
