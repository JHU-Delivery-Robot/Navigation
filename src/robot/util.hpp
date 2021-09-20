#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>

constexpr double PI = 3.14159265358979323846;

inline float degreesToRadians(float degrees)
{
    return degrees * PI / 180.0;
}

#endif
