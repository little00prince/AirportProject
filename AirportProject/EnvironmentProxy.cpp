
#include "EnvironmentProxy.h"

EnvironmentProxy::EnvironmentProxy(std::shared_ptr<Environment> environment)
    : realEnvironment(environment),
    accessAllowed(true),
    cacheEnabled(true),
    windCache_(0),
    directionCache_(""),
    visibilityCache_(0),
    pressureCache_(0),
    windCached(false),
    directionCached(false),
    visibilityCached(false),
    pressureCached(false) {
}

void EnvironmentProxy::CheckAccess() {
    if (!accessAllowed) {
        throw std::runtime_error("Доступ запрещён");
    }
}

void EnvironmentProxy::SetAccessAllowed(bool allowed) {
    accessAllowed = allowed;
}

void EnvironmentProxy::SetCacheEnabled(bool enabled) {
    cacheEnabled = enabled;
}

double EnvironmentProxy::GetWindSpeed() {
    CheckAccess();

    if (!cacheEnabled) {
        return realEnvironment->GetWindSpeed();
    }

    if (!windCached) {
        windCache_ = realEnvironment->GetWindSpeed();
        windCached = true;
        std::cout << "[Proxy] Скорость ветра загружена в кэш\n";
    }
    else {
        std::cout << "[Proxy] Скорость ветра получена из кэша\n";
    }

    return windCache_;
}

std::string EnvironmentProxy::GetWindDirection() {
    CheckAccess();

    if (!cacheEnabled) {
        return realEnvironment->GetWindDirection();
    }

    if (!directionCached) {
        directionCache_ = realEnvironment->GetWindDirection();
        directionCached = true;
    }

    return directionCache_;
}

double EnvironmentProxy::GetVisibility() {
    CheckAccess();

    if (!cacheEnabled) {
        return realEnvironment->GetVisibility();
    }

    if (!visibilityCached) {
        visibilityCache_ = realEnvironment->GetVisibility();
        visibilityCached = true;
    }

    return visibilityCache_;
}

double EnvironmentProxy::GetPressure() {
    CheckAccess();

    if (!cacheEnabled) {
        return realEnvironment->GetPressure();
    }

    if (!pressureCached) {
        pressureCache_ = realEnvironment->GetPressure();
        pressureCached = true;
    }

    return pressureCache_;
}
