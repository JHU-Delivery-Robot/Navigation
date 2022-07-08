#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <memory>

namespace hal {

class Motor {
public:
    enum class Status { NOMINAL, LOCKED, HOT };

    virtual ~Motor() { };

    virtual Status status() const = 0;
    virtual void set_speed(double speed) = 0;
    virtual double get_speed() const = 0;

    virtual void reset_odometry() = 0;
    virtual double odometry_distance() const = 0;
};

}

#endif
