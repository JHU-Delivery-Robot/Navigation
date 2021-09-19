/**
 * Type declaration files for this project
 */
#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>

typedef int32_t Speed;
typedef int32_t AngVel;
typedef int32_t Coord; // signed or unsigned?

/**
 * Container class for untility constants. All distance measurements are in
 * centimeters unless notated otherwise
 */
struct Constants {
    static constexpr int GRADIENT_SCALE = 1;
    static constexpr int LIDAR_N_ANGLES = 720;
    static constexpr int Q_STAR = 10;
    static constexpr float WHEEL_DIAMETER = 7.62;
};


/**
 * 2d Vector object
 */
struct Vec2d {
    Coord x;
    Coord y;

    Vec2d operator+(const Vec2d& vec) {
        Vec2d result;
        result.x = this->x + vec.x;
        result.y = this->y + vec.y;
        return result;
    }
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