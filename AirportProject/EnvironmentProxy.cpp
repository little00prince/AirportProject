#include "EnvironmentProxy.h"

EnvironmentProxy::EnvironmentProxy(std::shared_ptr<Environment> environment)
    : Environment(0.0, "", 0.0, 0.0),
    realEnvironment_(std::move(environment)),
    accessAllowed_(true),
    cacheEnabled_(true),
    windCache_(0),
    directionCache_(""),
    visibilityCache_(0),
    pressureCache_(0),
    windCached_(false),
    directionCached_(false),
    visibilityCached_(false),
    pressureCached_(false) {
}

void EnvironmentProxy::CheckAccess() {
    if (!accessAllowed_) {
        std::cout << "[EnvironmentProxy] ДОСТУП ЗАПРЕЩЁН — запрос отклонён\n";
        throw std::runtime_error("Доступ запрещен");
    }
}

void EnvironmentProxy::SetAccessAllowed(bool allowed) {
    accessAllowed_ = allowed;
    if (allowed) {
        std::cout << "[EnvironmentProxy] Режим доступа: РАЗРЕШЁН\n";
    }
    else {
        std::cout << "[EnvironmentProxy] Режим доступа: ЗАПРЕЩЁН\n";
    }
}

void EnvironmentProxy::SetCacheEnabled(bool enabled) {
    cacheEnabled_ = enabled;
    if (enabled) {
        std::cout << "[EnvironmentProxy] Кэширование: ВКЛЮЧЕНО\n";
    }
    else {
        std::cout << "[EnvironmentProxy] Кэширование: ВЫКЛЮЧЕНО\n";
    }
}

double EnvironmentProxy::GetWindSpeed() {
    CheckAccess();

    if (!cacheEnabled_) {
        std::cout << "[EnvironmentProxy] Скорость ветра ← реальный объект (кэш выкл.)\n";
        return realEnvironment_->GetWindSpeed();
    }

    if (!windCached_) {
        windCache_ = realEnvironment_->GetWindSpeed();
        windCached_ = true;
        std::cout << "[EnvironmentProxy] Скорость ветра ← реальный объект → в кэш\n";
    }
    else {
        std::cout << "[EnvironmentProxy] Скорость ветра ← кэш\n";
    }

    return windCache_;
}

std::string EnvironmentProxy::GetWindDirection() {
    CheckAccess();

    if (!cacheEnabled_) {
        std::cout << "[EnvironmentProxy] Направление ветра ← реальный объект (кэш выкл.)\n";
        return realEnvironment_->GetWindDirection();
    }

    if (!directionCached_) {
        directionCache_ = realEnvironment_->GetWindDirection();
        directionCached_ = true;
        std::cout << "[EnvironmentProxy] Направление ветра ← реальный объект → в кэш\n";
    }
    else {
        std::cout << "[EnvironmentProxy] Направление ветра ← кэш\n";
    }

    return directionCache_;
}

double EnvironmentProxy::GetVisibility() {
    CheckAccess();

    if (!cacheEnabled_) {
        std::cout << "[EnvironmentProxy] Видимость ← реальный объект (кэш выкл.)\n";
        return realEnvironment_->GetVisibility();
    }

    if (!visibilityCached_) {
        visibilityCache_ = realEnvironment_->GetVisibility();
        visibilityCached_ = true;
        std::cout << "[EnvironmentProxy] Видимость ← реальный объект → в кэш\n";
    }
    else {
        std::cout << "[EnvironmentProxy] Видимость ← кэш\n";
    }

    return visibilityCache_;
}

double EnvironmentProxy::GetPressure() {
    CheckAccess();

    if (!cacheEnabled_) {
        std::cout << "[EnvironmentProxy] Давление ← реальный объект (кэш выкл.)\n";
        return realEnvironment_->GetPressure();
    }

    if (!pressureCached_) {
        pressureCache_ = realEnvironment_->GetPressure();
        pressureCached_ = true;
        std::cout << "[EnvironmentProxy] Давление ← реальный объект → в кэш\n";
    }
    else {
        std::cout << "[EnvironmentProxy] Давление ← кэш\n";
    }

    return pressureCache_;
}
