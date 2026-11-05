#pragma once
#include "Aircraft.h"
#include "GroundService.h"

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
    explicit ServiceManager(GroundService groundService);
    void RequestServices(const Aircraft& aircraft);

private:
    GroundService groundService_;
};