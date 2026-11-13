#pragma once
#include <string>
#include <memory>
#include <map>

// Flyweight: разделяемое внутреннее состояние типа ВС (intrinsic)
class AircraftType {
public:
    AircraftType(std::string typeName, std::string category);

    const std::string& GetTypeName() const;
    const std::string& GetCategory() const;

private:
    std::string typeName_;
    std::string category_;
};

// Фабрика / пул легковесов
class AircraftTypeFactory {
public:
    static std::shared_ptr<AircraftType> GetType(const std::string& typeName,
        const std::string& category);
    static size_t PoolSize();

private:
    static std::map<std::string, std::shared_ptr<AircraftType> > pool_;
};
