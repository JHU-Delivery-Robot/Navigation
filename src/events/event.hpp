#ifndef MESSAGE_HPP
#define MESSAGE_HPP

namespace events {

class Event {
public:
    static constexpr int LOW_PRIORITY = 1;
    static constexpr int HIGH_PRIORITY = 0;

    enum class Type { FATAL_ERROR,
                      ROUTE_UPDATE };

    Event(int priority, Type type) : priority(priority), type(type) {};

    virtual ~Event(){};
    int getPriority() const { return priority; };
    Type getType() const { return type; };

private:
    int priority;
    Type type;
};

}  // namespace events

#endif
