#include <cmath>
#include "hal.hpp"
#include "poten_map.hpp"
#include "motors.hpp"

void velocity_to_motor_output(Vector cartesian_velocity) {
    velocity_magnitude = sqrt((float)(cartesian_velocity.x) * (float)(cartesian_velocity.x) +
                         (float)(cartesian_velocity.y) * (float)(cartesian_velocity.y));
    velocity_angle = atan2((float)(cartesian_velocity.y), (float)(cartesian_velocity.x));

    // robot_motors.front_l.set_speed();
    // robot_motors.back_l.set_speed();
    // robot_motors.front_r.set_speed();
    // robot_motors.back_r.set_speed();
}