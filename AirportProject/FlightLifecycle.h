#pragma once
#include "Aircraft.h"
#include <memory>
#include <string>
#include <vector>

// ---------- Observer ----------
class IFlightObserver {
public:
    virtual ~IFlightObserver() = default;
    virtual void OnFlightEvent(const std::string& flightNumber,
        const std::string& stateName) = 0;
};

class ConsoleBoardObserver : public IFlightObserver {
public:
    void OnFlightEvent(const std::string& flightNumber,
        const std::string& stateName) override;
};

class GroundOpsObserver : public IFlightObserver {
public:
    void OnFlightEvent(const std::string& flightNumber,
        const std::string& stateName) override;
};

// ---------- State ----------
class FlightLifecycle;

class IFlightState {
public:
    virtual ~IFlightState() = default;
    virtual void Handle(FlightLifecycle& lifecycle) = 0;
    virtual std::string Name() const = 0;
};

class RegisteredState : public IFlightState {
public:
    void Handle(FlightLifecycle& lifecycle) override;
    std::string Name() const override;
};

class OnApproachState : public IFlightState {
public:
    void Handle(FlightLifecycle& lifecycle) override;
    std::string Name() const override;
};

class AtGateState : public IFlightState {
public:
    void Handle(FlightLifecycle& lifecycle) override;
    std::string Name() const override;
};

class CompletedState : public IFlightState {
public:
    void Handle(FlightLifecycle& lifecycle) override;
    std::string Name() const override;
};

// Контекст State + субъект Observer
class FlightLifecycle {
public:
    explicit FlightLifecycle(const Aircraft& aircraft);

    void Attach(std::shared_ptr<IFlightObserver> observer);
    void Advance();              // делегирует текущему состоянию
    void RunToCompletion();      // крутит Advance, пока не Completed

    void ChangeState(std::shared_ptr<IFlightState> newState);
    const Aircraft& GetAircraft() const;
    std::string CurrentStateName() const;
    bool IsCompleted() const;

private:
    void NotifyObservers();

    const Aircraft& aircraft_;
    std::shared_ptr<IFlightState> state_;
    std::vector<std::shared_ptr<IFlightObserver> > observers_;
    bool completed_;
};
