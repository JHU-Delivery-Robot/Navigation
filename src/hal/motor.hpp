#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <memory>

namespace hal {

class Motor {
public:
    enum class Status { NOMINAL, LOCKED, HOT };

    virtual ~Motor() { };

    virtual Status status() = 0;
    virtual void set_speed(float speed) = 0;
    virtual float get_speed() = 0;

    virtual void reset_odometry() = 0;
    virtual double odometry_distance() = 0;
};

}

#endif
