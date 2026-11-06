#pragma once
#include "Aircraft.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

// ========== Composite: общий интерфейс службы ==========
class IService {
public:
    virtual ~IService() = default;
    virtual void Serve(const Aircraft& aircraft) = 0;
    virtual std::string GetName() const = 0;
};

// ========== Листовые службы ==========
class FuelService : public IService {
public:
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override { return "Заправка"; }
};

class BaggageService : public IService {
public:
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override { return "Багаж"; }
};

class CleaningService : public IService {
public:
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override { return "Уборка"; }
};

class CateringService : public IService {
public:
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override { return "Питание"; }
};

// ========== Итератор ==========
class ServiceIterator {
public:
    explicit ServiceIterator(const std::vector<std::shared_ptr<IService>>& services);
    bool HasNext() const;
    std::shared_ptr<IService> Next();

private:
    const std::vector<std::shared_ptr<IService>>& services_;
    size_t index_;
};

// ========== Компоновщик ==========
class CompositeService : public IService {
public:
    CompositeService();
    explicit CompositeService(std::string name);

    void AddService(std::shared_ptr<IService> service);
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override;
    ServiceIterator CreateIterator() const;
    size_t Count() const { return services_.size(); }

private:
    std::string name_;
    std::vector<std::shared_ptr<IService>> services_;
};

// ========== Иерархия декораторов ==========
class ServiceDecorator : public IService {
public:
    explicit ServiceDecorator(std::shared_ptr<IService> wrappee);
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override;

protected:
    std::shared_ptr<IService> wrappee_;
};

class LoggingServiceDecorator : public ServiceDecorator {
public:
    explicit LoggingServiceDecorator(std::shared_ptr<IService> wrappee);
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override;
};

class TimingServiceDecorator : public ServiceDecorator {
public:
    explicit TimingServiceDecorator(std::shared_ptr<IService> wrappee);
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override;
};

class PriorityServiceDecorator : public ServiceDecorator {
public:
    explicit PriorityServiceDecorator(std::shared_ptr<IService> wrappee);
    void Serve(const Aircraft& aircraft) override;
    std::string GetName() const override;
};
