#ifndef TYPES_HPP
#define TYPES_HPP

/**
 * Type declaration files for this project
 */

/**
 * Container class for untility constants. All distance measurements are in
 * centimeters unless notated otherwise
 */
struct Constants
{
    static constexpr int GRADIENT_SCALE = 1;
    static constexpr int LIDAR_N_ANGLES = 720;
    static constexpr int Q_STAR = 10;
    static constexpr float WHEEL_DIAMETER = 7.62;
};

#endif /* TYPES_HPP */
