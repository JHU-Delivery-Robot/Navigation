/**
 * Type declaration files for this project
 */
#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>

typedef int32_t Speed;
typedef int32_t AngVel;

/**
 * Container class for untility constants. All distance measurements are in
 * centimeters unless notated otherwise
 */
struct Constants {
    static constexpr float WHEEL_DIAMETER = 7.62;
};

struct MotorPosition {
    float theta;
    float r;
};

struct MotorPositions {
    MotorPosition front_l;
    MotorPosition front_r;
    MotorPosition back_l;
    MotorPosition back_r;

    MotorPositions() {
    }
};

#endif /* TYPES_HPP */
