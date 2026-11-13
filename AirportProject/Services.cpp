#include "Services.h"

// ---------- Листовые службы ----------
void FuelService::Serve(const Aircraft& aircraft) {
    std::cout << "[FuelService] Заправка воздушного судна "
        << aircraft.GetFlightNumber()
        << " (" << aircraft.GetType() << ")\n";
}

void BaggageService::Serve(const Aircraft& aircraft) {
    std::cout << "[BaggageService] Выгрузка багажа с рейса "
        << aircraft.GetFlightNumber() << "\n";
    std::cout << "[BaggageService] Погрузка багажа на рейс "
        << aircraft.GetFlightNumber() << "\n";
}

void CleaningService::Serve(const Aircraft& aircraft) {
    std::cout << "[CleaningService] Уборка салона "
        << aircraft.GetFlightNumber() << "\n";
}

void CateringService::Serve(const Aircraft& aircraft) {
    std::cout << "[CateringService] Пополнение бортового питания рейса "
        << aircraft.GetFlightNumber() << "\n";
}

void MedicalService::Serve(const Aircraft& aircraft) {
    std::cout << "[MedicalService] Аварийная медбригада встречает "
        << aircraft.GetFlightNumber() << "\n";
    std::cout << "[MedicalService] Приоритетная эвакуация / осмотр на борту\n";
}

// ---------- Итератор ----------
ServiceIterator::ServiceIterator(const std::vector<std::shared_ptr<IService> >& services)
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
    // Обход дочерних служб через Iterator (не напрямую по vector)
    ServiceIterator it = CreateIterator();
    while (it.HasNext()) {
        std::shared_ptr<IService> service = it.Next();
        std::cout << "[" << name_ << "] -> " << service->GetName() << "\n";
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
    return wrappee_ ? wrappee_->GetName() : "EmptyDecorator";
}

// ---------- LoggingServiceDecorator ----------
LoggingServiceDecorator::LoggingServiceDecorator(std::shared_ptr<IService> wrappee)
    : ServiceDecorator(std::move(wrappee)) {}

void LoggingServiceDecorator::Serve(const Aircraft& aircraft) {
    std::cout << "[LoggingServiceDecorator] >>> начало: " << GetName()
        << " для " << aircraft.GetFlightNumber() << "\n";
    ServiceDecorator::Serve(aircraft);
    std::cout << "[LoggingServiceDecorator] <<< конец:  " << GetName()
        << " для " << aircraft.GetFlightNumber() << "\n";
}

std::string LoggingServiceDecorator::GetName() const {
    return "LoggingServiceDecorator(" + ServiceDecorator::GetName() + ")";
}

// ---------- TimingServiceDecorator ----------
TimingServiceDecorator::TimingServiceDecorator(std::shared_ptr<IService> wrappee)
    : ServiceDecorator(std::move(wrappee)) {}

void TimingServiceDecorator::Serve(const Aircraft& aircraft) {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    ServiceDecorator::Serve(aircraft);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "[TimingServiceDecorator] " << GetName()
        << " заняло " << ms << " мс\n";
}

std::string TimingServiceDecorator::GetName() const {
    return "TimingServiceDecorator(" + ServiceDecorator::GetName() + ")";
}

// ---------- PriorityServiceDecorator ----------
PriorityServiceDecorator::PriorityServiceDecorator(std::shared_ptr<IService> wrappee)
    : ServiceDecorator(std::move(wrappee)) {}

void PriorityServiceDecorator::Serve(const Aircraft& aircraft) {
    if (aircraft.GetCategory() == "аварийный") {
        std::cout << "[PriorityServiceDecorator] АВАРИЙНЫЙ режим для "
            << aircraft.GetFlightNumber()
            << " — ускоренное обслуживание\n";
    }
    ServiceDecorator::Serve(aircraft);
}

std::string PriorityServiceDecorator::GetName() const {
    return "PriorityServiceDecorator(" + ServiceDecorator::GetName() + ")";
}
