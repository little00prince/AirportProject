#pragma once

#include "Environment.h"
#include <string>
#include <memory>
#include <iostream>
#include <stdexcept>

class EnvironmentProxy : public IEnvironment {
private:
    std::shared_ptr<Environment> realEnvironment;

    bool accessAllowed;
    bool cacheEnabled;

    double windCache_;
    std::string directionCache_;
    double visibilityCache_;
    double pressureCache_;

    bool windCached;
    bool directionCached;
    bool visibilityCached;
    bool pressureCached;

    void CheckAccess();

public:
    explicit EnvironmentProxy(std::shared_ptr<Environment> environment);

    void SetAccessAllowed(bool allowed);
    void SetCacheEnabled(bool enabled);

    double GetWindSpeed() override;
    std::string GetWindDirection() override;
    double GetVisibility() override;
    double GetPressure() override;
};
