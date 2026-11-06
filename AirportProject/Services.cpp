#include "Services.h"
#include <thread>

// ---------- Листовые службы ----------
void FuelService::Serve(const Aircraft& aircraft) {
    std::cout << "[Заправка] Заправка воздушного судна "
        << aircraft.GetFlightNumber()
        << " (" << aircraft.GetType() << ")\n";
}

void BaggageService::Serve(const Aircraft& aircraft) {
    std::cout << "[Багаж] Выгрузка багажа с рейса "
        << aircraft.GetFlightNumber() << "\n";
    std::cout << "[Багаж] Погрузка багажа на рейс "
        << aircraft.GetFlightNumber() << "\n";
}

void CleaningService::Serve(const Aircraft& aircraft) {
    std::cout << "[Уборка] Уборка салона воздушного судна "
        << aircraft.GetFlightNumber() << "\n";
}

void CateringService::Serve(const Aircraft& aircraft) {
    std::cout << "[Питание] Пополнение бортового питания рейса "
        << aircraft.GetFlightNumber() << "\n";
}

// ---------- Итератор ----------
ServiceIterator::ServiceIterator(const std::vector<std::shared_ptr<IService>>& services)
    : services_(services), index_(0) {}

bool ServiceIterator::HasNext() const {
    return index_ < services_.size();
}

std::shared_ptr<IService> ServiceIterator::Next() {
    return services_[index_++];
}

// ---------- Компоновщик ----------
CompositeService::CompositeService()
    : name_("CompositeService") {}

CompositeService::CompositeService(std::string name)
    : name_(std::move(name)) {}

void CompositeService::AddService(std::shared_ptr<IService> service) {
    services_.push_back(std::move(service));
}

void CompositeService::Serve(const Aircraft& aircraft) {
    std::cout << "[" << name_ << "] Начало обслуживания рейса "
        << aircraft.GetFlightNumber() << "\n";
    for (const auto& service : services_) {
        service->Serve(aircraft);
    }
    std::cout << "[" << name_ << "] Обслуживание рейса "
        << aircraft.GetFlightNumber() << " завершено\n";
}

std::string CompositeService::GetName() const {
    return name_;
}

ServiceIterator CompositeService::CreateIterator() const {
    return ServiceIterator(services_);
}

// ---------- Базовый декоратор ----------
ServiceDecorator::ServiceDecorator(std::shared_ptr<IService> wrappee)
    : wrappee_(std::move(wrappee)) {}

void ServiceDecorator::Serve(const Aircraft& aircraft) {
    if (wrappee_) {
        wrappee_->Serve(aircraft);
    }
}

std::string ServiceDecorator::GetName() const {
    return wrappee_ ? wrappee_->GetName() : "ПустойДекоратор";
}

// ---------- Декоратор журналирования ----------
LoggingServiceDecorator::LoggingServiceDecorator(std::shared_ptr<IService> wrappee)
    : ServiceDecorator(std::move(wrappee)) {}

void LoggingServiceDecorator::Serve(const Aircraft& aircraft) {
    std::cout << "[Журнал] >>> начало: " << GetName()
        << " для " << aircraft.GetFlightNumber() << "\n";
    ServiceDecorator::Serve(aircraft);
    std::cout << "[Журнал] <<< конец:  " << GetName()
        << " для " << aircraft.GetFlightNumber() << "\n";
}

std::string LoggingServiceDecorator::GetName() const {
    return "Журнал(" + ServiceDecorator::GetName() + ")";
}

// ---------- Декоратор замера времени ----------
TimingServiceDecorator::TimingServiceDecorator(std::shared_ptr<IService> wrappee)
    : ServiceDecorator(std::move(wrappee)) {}

void TimingServiceDecorator::Serve(const Aircraft& aircraft) {
    auto start = std::chrono::steady_clock::now();
    ServiceDecorator::Serve(aircraft);
    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "[Замер] " << GetName() << " заняло " << ms << " мс\n";
}

std::string TimingServiceDecorator::GetName() const {
    return "Замер(" + ServiceDecorator::GetName() + ")";
}

// ---------- Декоратор приоритета ----------
PriorityServiceDecorator::PriorityServiceDecorator(std::shared_ptr<IService> wrappee)
    : ServiceDecorator(std::move(wrappee)) {}

void PriorityServiceDecorator::Serve(const Aircraft& aircraft) {
    if (aircraft.GetCategory() == "аварийный") {
        std::cout << "[Приоритет] АВАРИЙНЫЙ режим для "
            << aircraft.GetFlightNumber()
            << " — ускоренное обслуживание\n";
    }
    ServiceDecorator::Serve(aircraft);
}

std::string PriorityServiceDecorator::GetName() const {
    return "Приоритет(" + ServiceDecorator::GetName() + ")";
}
