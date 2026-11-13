#include "Announcement.h"
#include <iostream>

void ConsoleAnnouncementImplementor::Broadcast(const std::string& message) {
    std::cout << "[ConsoleAnnouncementImplementor] " << message << "\n";
}

void PriorityAnnouncementImplementor::Broadcast(const std::string& message) {
    std::cout << "[PriorityAnnouncementImplementor] !!! " << message << " !!!\n";
}

Announcement::Announcement(std::shared_ptr<IAnnouncementImplementor> implementor)
    : implementor_(std::move(implementor)) {}

void Announcement::SetImplementor(std::shared_ptr<IAnnouncementImplementor> implementor) {
    implementor_ = std::move(implementor);
}

void Announcement::AnnounceArrival(const Aircraft& aircraft) {
    if (!implementor_) {
        return;
    }
    std::string msg = "Прибытие рейса " + aircraft.GetFlightNumber()
        + " (" + aircraft.GetType() + ")";
    implementor_->Broadcast(msg);
}

DetailedAnnouncement::DetailedAnnouncement(
    std::shared_ptr<IAnnouncementImplementor> implementor)
    : Announcement(std::move(implementor)) {}

void DetailedAnnouncement::AnnounceArrival(const Aircraft& aircraft) {
    if (!implementor_) {
        return;
    }
    std::string msg = "Рейс " + aircraft.GetFlightNumber()
        + ", тип: " + aircraft.GetType()
        + ", категория: " + aircraft.GetCategory();
    implementor_->Broadcast(msg);
}
