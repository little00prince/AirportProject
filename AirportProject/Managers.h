#pragma once
#include "Aircraft.h"
#include <iostream>
#include <string>

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
    void RequestServices(const Aircraft& aircraft);
};
