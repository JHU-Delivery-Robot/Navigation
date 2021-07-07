#include <cmath>
#include "hal.hpp"
#include "poten_map.hpp"
#include "motors.hpp"

Speed ang_vel_to_motor(AngVel ang_vel, Motor mtr) {
  return WHEEL_DIAMETER * ang_vel * cos(mtr.theta() / mtr.r();
}

void velocity_to_motor_output(Speed speed, AngVel ang_vel) {
    robot_motors.front_l.set_speed(speed, ang_vel_to_motor(ang_vel, robot_motors.front_l));
    robot_motors.back_l.set_speed(speed, ang_vel_to_motor(ang_vel, robot_motors.back_l));
    robot_motors.front_r.set_speed(speed, ang_vel_to_motor(ang_vel, robot_motors.front_r));
    robot_motors.back_r.set_speed(speed, ang_vel_to_motor(ang_vel, robot_motors.back_r));
}
