#include "AircraftType.h"
#include <iostream>

AircraftType::AircraftType(std::string typeName, std::string category)
    : typeName_(std::move(typeName)), category_(std::move(category)) {}

const std::string& AircraftType::GetTypeName() const {
    return typeName_;
}

const std::string& AircraftType::GetCategory() const {
    return category_;
}

std::map<std::string, std::shared_ptr<AircraftType> > AircraftTypeFactory::pool_;

std::shared_ptr<AircraftType> AircraftTypeFactory::GetType(
    const std::string& typeName,
    const std::string& category)
{
    std::string key = typeName + "|" + category;
    std::map<std::string, std::shared_ptr<AircraftType> >::iterator it = pool_.find(key);
    if (it != pool_.end()) {
        std::cout << "[AircraftTypeFactory] Повторное использование Flyweight: "
            << typeName << " (в пуле: " << pool_.size() << ")\n";
        return it->second;
    }

    std::shared_ptr<AircraftType> created(new AircraftType(typeName, category));
    pool_[key] = created;
    std::cout << "[AircraftTypeFactory] Создан новый Flyweight: "
        << typeName << " (в пуле: " << pool_.size() << ")\n";
    return created;
}

size_t AircraftTypeFactory::PoolSize() {
    return pool_.size();
}
