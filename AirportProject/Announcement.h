#pragma once
#include "Aircraft.h"
#include <memory>
#include <string>

// Bridge: реализация (implementor)
class IAnnouncementImplementor {
public:
    virtual ~IAnnouncementImplementor() = default;
    virtual void Broadcast(const std::string& message) = 0;
};

class ConsoleAnnouncementImplementor : public IAnnouncementImplementor {
public:
    void Broadcast(const std::string& message) override;
};

class PriorityAnnouncementImplementor : public IAnnouncementImplementor {
public:
    void Broadcast(const std::string& message) override;
};

// Bridge: абстракция
class Announcement {
public:
    explicit Announcement(std::shared_ptr<IAnnouncementImplementor> implementor);
    virtual ~Announcement() = default;

    void SetImplementor(std::shared_ptr<IAnnouncementImplementor> implementor);
    virtual void AnnounceArrival(const Aircraft& aircraft);

protected:
    std::shared_ptr<IAnnouncementImplementor> implementor_;
};

// Расширенная абстракция
class DetailedAnnouncement : public Announcement {
public:
    explicit DetailedAnnouncement(std::shared_ptr<IAnnouncementImplementor> implementor);
    void AnnounceArrival(const Aircraft& aircraft) override;
};
