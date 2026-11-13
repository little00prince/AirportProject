#pragma once

#include "Environment.h"
#include <string>
#include <memory>
#include <iostream>
#include <stdexcept>

class EnvironmentProxy : public Environment {
public:
    explicit EnvironmentProxy(std::shared_ptr<Environment> environment);

    void SetAccessAllowed(bool allowed) override;
    void SetCacheEnabled(bool enabled) override;

    double GetWindSpeed() override;
    std::string GetWindDirection() override;
    double GetVisibility() override;
    double GetPressure() override;

private:
    void CheckAccess();

    std::shared_ptr<Environment> realEnvironment_;

    bool accessAllowed_;
    bool cacheEnabled_;

    double windCache_;
    std::string directionCache_;
    double visibilityCache_;
    double pressureCache_;

    bool windCached_;
    bool directionCached_;
    bool visibilityCached_;
    bool pressureCached_;
};