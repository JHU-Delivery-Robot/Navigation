#ifndef MOTOR_SIM_IMPL_HPP
#define MOTOR_SIM_IMPL_HPP

#include "hal/motor.hpp"
#include "sim/simulation.hpp"

namespace sim {

class MotorSimImpl final : public hal::Motor {
public:
    MotorSimImpl();
    void update(double time_delta);

    hal::Motor::Status status() const override;
    void set_speed(double speed) override;
    double get_speed() const override;

    void reset_odometry() override;
    double odometry_distance() const override;

private:
    double speed;
    double total_distance;
};

}

#endif
