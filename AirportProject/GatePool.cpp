#include "GatePool.h"
#include <iostream>

Gate::Gate(int number)
    : number_(number), free_(true), currentFlight_() {}

int Gate::Number() const {
    return number_;
}

bool Gate::IsFree() const {
    return free_;
}

void Gate::Occupy(const std::string& flightNumber) {
    free_ = false;
    currentFlight_ = flightNumber;
}

void Gate::Release() {
    free_ = true;
    currentFlight_.clear();
}

GatePool::GatePool(int capacity) {
    for (int i = 1; i <= capacity; ++i) {
        gates_.push_back(std::shared_ptr<Gate>(new Gate(i)));
    }
    std::cout << "[GatePool] Создан пул из " << capacity << " гейтов\n";
}

std::shared_ptr<Gate> GatePool::Acquire(const std::string& flightNumber) {
    for (size_t i = 0; i < gates_.size(); ++i) {
        if (gates_[i]->IsFree()) {
            gates_[i]->Occupy(flightNumber);
            std::cout << "[GatePool] Acquire: гейт " << gates_[i]->Number()
                << " выдан рейсу " << flightNumber
                << " (свободно: " << AvailableCount() << ")\n";
            return gates_[i];
        }
    }
    std::cout << "[GatePool] Acquire: нет свободных гейтов для рейса "
        << flightNumber << "\n";
    return std::shared_ptr<Gate>();
}

void GatePool::Release(std::shared_ptr<Gate> gate) {
    if (!gate) {
        return;
    }
    int num = gate->Number();
    gate->Release();
    std::cout << "[GatePool] Release: гейт " << num
        << " возвращён в пул (свободно: " << AvailableCount() << ")\n";
}

int GatePool::Capacity() const {
    return static_cast<int>(gates_.size());
}

int GatePool::AvailableCount() const {
    int count = 0;
    for (size_t i = 0; i < gates_.size(); ++i) {
        if (gates_[i]->IsFree()) {
            ++count;
        }
    }
    return count;
}
