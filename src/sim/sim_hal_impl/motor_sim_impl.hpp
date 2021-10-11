#ifndef MOTOR_SIM_IMPL_HPP
#define MOTOR_SIM_IMPL_HPP

#include "motor.hpp"

namespace sim {

class MotorSimImpl final : public hal::Motor {
public:
    MotorSimImpl();
    void update(double time_delta);

    hal::Motor::Status status() override;
    void set_speed(float speed) override;
    float get_speed() override;

    void reset_odometry() override;
    double odometry_distance() override;

private:
    float speed;
    double total_distance;
};

}

#endif
