#ifndef MESSAGE_HPP
#define MESSAGE_HPP

namespace events {

class Event {
public:
    Event(int priority) : priority(priority){};

    virtual ~Event(){};
    int priority;
};

}  // namespace events

#endif
