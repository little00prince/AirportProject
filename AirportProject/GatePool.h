#pragma once
#include <memory>
#include <string>
#include <vector>

// Объект пула — гейт стоянки
class Gate {
public:
    explicit Gate(int number);

    int Number() const;
    bool IsFree() const;
    void Occupy(const std::string& flightNumber);
    void Release();

private:
    int number_;
    bool free_;
    std::string currentFlight_;
};

// Object Pool: ограниченный набор переиспользуемых гейтов
class GatePool {
public:
    explicit GatePool(int capacity);

    // Взять свободный гейт из пула (или nullptr, если все заняты)
    std::shared_ptr<Gate> Acquire(const std::string& flightNumber);

    // Вернуть гейт в пул
    void Release(std::shared_ptr<Gate> gate);

    int Capacity() const;
    int AvailableCount() const;

private:
    std::vector<std::shared_ptr<Gate> > gates_;
};
