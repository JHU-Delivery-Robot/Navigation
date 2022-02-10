#ifndef ERROR_REPORTING_HPP
#define ERROR_REPORTING_HPP

#include <memory>

#include "event_queue.hpp"

namespace events {

class ErrorReporting {
public:
    ErrorReporting(EventQueue* event_queue);

    void reportError(std::string origin, std::string details);

private:
    EventQueue* event_queue;
};

}  // namespace events

#endif
