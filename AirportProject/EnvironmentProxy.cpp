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
        std::cout << "[Proxy] ДОСТУП ЗАПРЕЩЁН — запрос к данным окружения отклонён\n";
        throw std::runtime_error("Доступ запрещен");
    }
}

void EnvironmentProxy::SetAccessAllowed(bool allowed) {
    accessAllowed_ = allowed;
    if (allowed) {
        std::cout << "[Proxy] Режим доступа: РАЗРЕШЁН\n";
    }
    else {
        std::cout << "[Proxy] Режим доступа: ЗАПРЕЩЁН\n";
    }
}

void EnvironmentProxy::SetCacheEnabled(bool enabled) {
    cacheEnabled_ = enabled;
    if (enabled) {
        std::cout << "[Proxy] Кэширование: ВКЛЮЧЕНО\n";
    }
    else {
        std::cout << "[Proxy] Кэширование: ВЫКЛЮЧЕНО\n";
    }
}

double EnvironmentProxy::GetWindSpeed() {
    CheckAccess();

    if (!cacheEnabled_) {
        std::cout << "[Proxy] Скорость ветра ← реальный объект (кэш выкл.)\n";
        return realEnvironment_->GetWindSpeed();
    }

    if (!windCached_) {
        windCache_ = realEnvironment_->GetWindSpeed();
        windCached_ = true;
        std::cout << "[Proxy] Скорость ветра ← реальный объект → сохранено в кэш\n";
    }
    else {
        std::cout << "[Proxy] Скорость ветра ← кэш\n";
    }

    return windCache_;
}

std::string EnvironmentProxy::GetWindDirection() {
    CheckAccess();

    if (!cacheEnabled_) {
        std::cout << "[Proxy] Направление ветра ← реальный объект (кэш выкл.)\n";
        return realEnvironment_->GetWindDirection();
    }

    if (!directionCached_) {
        directionCache_ = realEnvironment_->GetWindDirection();
        directionCached_ = true;
        std::cout << "[Proxy] Направление ветра ← реальный объект → сохранено в кэш\n";
    }
    else {
        std::cout << "[Proxy] Направление ветра ← кэш\n";
    }

    return directionCache_;
}

double EnvironmentProxy::GetVisibility() {
    CheckAccess();

    if (!cacheEnabled_) {
        std::cout << "[Proxy] Видимость ← реальный объект (кэш выкл.)\n";
        return realEnvironment_->GetVisibility();
    }

    if (!visibilityCached_) {
        visibilityCache_ = realEnvironment_->GetVisibility();
        visibilityCached_ = true;
        std::cout << "[Proxy] Видимость ← реальный объект → сохранено в кэш\n";
    }
    else {
        std::cout << "[Proxy] Видимость ← кэш\n";
    }

    return visibilityCache_;
}

double EnvironmentProxy::GetPressure() {
    CheckAccess();

    if (!cacheEnabled_) {
        std::cout << "[Proxy] Давление ← реальный объект (кэш выкл.)\n";
        return realEnvironment_->GetPressure();
    }

    if (!pressureCached_) {
        pressureCache_ = realEnvironment_->GetPressure();
        pressureCached_ = true;
        std::cout << "[Proxy] Давление ← реальный объект → сохранено в кэш\n";
    }
    else {
        std::cout << "[Proxy] Давление ← кэш\n";
    }

    return pressureCache_;
}
