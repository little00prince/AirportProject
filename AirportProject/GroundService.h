#pragma once
#include "Aircraft.h"

class FuelService {
public:
    void Refuel(const Aircraft& aircraft);
};

class BaggageService {
public:
    void UnloadBaggage(const Aircraft& aircraft);
    void LoadBaggage(const Aircraft& aircraft);
};

class CleaningService {
public:
    void CleanCabin(const Aircraft& aircraft);
};

class CateringService {
public:
    void ReplenishSupplies(const Aircraft& aircraft);
};

// Набор услуг, которые выполняет наземная служба
struct ServicePackage {
    bool fuel = true;
    bool baggage = true;
    bool cleaning = true;
    bool catering = true;
};

class GroundService {
public:
    GroundService() = default;
    explicit GroundService(ServicePackage package) : package_(package) {}

    void ServeAircraft(const Aircraft& aircraft);

private:
    FuelService fuelService_;
    BaggageService baggageService_;
    CleaningService cleaningService_;
    CateringService cateringService_;
    ServicePackage package_;
};