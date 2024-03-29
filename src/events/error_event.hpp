#ifndef ERROR_EVENT_HPP
#define ERROR_EVENT_HPP

#include <string>

#include "event.hpp"

namespace events {

class ErrorEvent : public Event {
public:
    ErrorEvent(std::string origin, std::string details, int priority);

    std::string getOrigin() const;
    std::string getDetails() const;

private:
    std::string origin;
    std::string details;
};

}  // namespace events

#endif
