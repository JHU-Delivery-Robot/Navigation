#ifndef MESSAGE_HPP
#define MESSAGE_HPP

namespace events {

class Event {
public:
    static constexpr int LOW_PRIORITY = 1;
    static constexpr int HIGH_PRIORITY = 0;

    Event(int priority) : priority(priority){};

    virtual ~Event(){};
    int getPriority() const { return priority; };

private:
    int priority;
};

}  // namespace events

#endif
