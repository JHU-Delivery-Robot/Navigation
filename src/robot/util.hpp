#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>

constexpr double M_PI = 3.14159265358979323846;

inline float degreesToRadians(float degrees)
{
    return degrees * M_PI / 180.0;
}

#endif
