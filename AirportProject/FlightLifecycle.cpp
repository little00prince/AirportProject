#include "FlightLifecycle.h"
#include <iostream>

// ----- Observer -----
void ConsoleBoardObserver::OnFlightEvent(const std::string& flightNumber,
    const std::string& stateName) {
    std::cout << "[ConsoleBoardObserver] Табло: рейс " << flightNumber
        << " -> состояние \"" << stateName << "\"\n";
}

void GroundOpsObserver::OnFlightEvent(const std::string& flightNumber,
    const std::string& stateName) {
    std::cout << "[GroundOpsObserver] Наземные службы уведомлены: "
        << flightNumber << " / " << stateName << "\n";
}

// ----- States -----
std::string RegisteredState::Name() const { return "Registered"; }

void RegisteredState::Handle(FlightLifecycle& lifecycle) {
    std::cout << "[RegisteredState] Рейс "
        << lifecycle.GetAircraft().GetFlightNumber()
        << " зарегистрирован в системе\n";
    lifecycle.ChangeState(std::shared_ptr<IFlightState>(new OnApproachState()));
}

std::string OnApproachState::Name() const { return "OnApproach"; }

void OnApproachState::Handle(FlightLifecycle& lifecycle) {
    std::cout << "[OnApproachState] Рейс "
        << lifecycle.GetAircraft().GetFlightNumber()
        << " на подходе\n";
    lifecycle.ChangeState(std::shared_ptr<IFlightState>(new AtGateState()));
}

std::string AtGateState::Name() const { return "AtGate"; }

void AtGateState::Handle(FlightLifecycle& lifecycle) {
    std::cout << "[AtGateState] Рейс "
        << lifecycle.GetAircraft().GetFlightNumber()
        << " у гейта, можно обслуживать\n";
    lifecycle.ChangeState(std::shared_ptr<IFlightState>(new CompletedState()));
}

std::string CompletedState::Name() const { return "Completed"; }

void CompletedState::Handle(FlightLifecycle& lifecycle) {
    std::cout << "[CompletedState] Обработка рейса "
        << lifecycle.GetAircraft().GetFlightNumber()
        << " завершена (конечное состояние)\n";
    // остаёмся в Completed — смену состояния не делаем
}

// ----- Context -----
FlightLifecycle::FlightLifecycle(const Aircraft& aircraft)
    : aircraft_(aircraft)
    , state_(std::shared_ptr<IFlightState>(new RegisteredState()))
    , completed_(false)
{
    std::cout << "[FlightLifecycle] Контекст создан, начальное состояние: "
        << state_->Name() << "\n";
}

void FlightLifecycle::Attach(std::shared_ptr<IFlightObserver> observer) {
    observers_.push_back(observer);
    std::cout << "[FlightLifecycle] Observer подключён (всего: "
        << observers_.size() << ")\n";
}

void FlightLifecycle::NotifyObservers() {
    std::string flight = aircraft_.GetFlightNumber();
    std::string name = state_ ? state_->Name() : "None";
    for (size_t i = 0; i < observers_.size(); ++i) {
        if (observers_[i]) {
            observers_[i]->OnFlightEvent(flight, name);
        }
    }
}

void FlightLifecycle::ChangeState(std::shared_ptr<IFlightState> newState) {
    state_ = newState;
    if (state_ && state_->Name() == "Completed") {
        completed_ = true;
    }
    std::cout << "[FlightLifecycle] Переход в состояние: "
        << (state_ ? state_->Name() : "None") << "\n";
    NotifyObservers();
}

void FlightLifecycle::Advance() {
    if (!state_) {
        return;
    }
    std::cout << "[FlightLifecycle] Advance из \"" << state_->Name() << "\"\n";
    state_->Handle(*this);
}

void FlightLifecycle::RunToCompletion() {
    // Registered -> OnApproach -> AtGate -> Completed: несколько Advance
    int guard = 0;
    while (!completed_ && guard < 10) {
        Advance();
        ++guard;
    }
    // финальный Advance в Completed (логирует конечное состояние)
    if (completed_) {
        Advance();
    }
}

const Aircraft& FlightLifecycle::GetAircraft() const {
    return aircraft_;
}

std::string FlightLifecycle::CurrentStateName() const {
    return state_ ? state_->Name() : "None";
}

bool FlightLifecycle::IsCompleted() const {
    return completed_;
}
