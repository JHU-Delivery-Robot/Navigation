#ifndef COORDINATE_SYSTEM_HPP
#define COORDINATE_SYSTEM_HPP

#include <iostream>

#include "coordinates.hpp"
#include "vector2.hpp"

namespace common {

/**
 Spherical transverse Mercator projection used to construct local Cartesian coordinate systems around arbitrary origins.

 Constructed coordinate systems are in meters, and are centered on the central meridian passing through the origin.
 To simplify math, coordinate system only valid for hemisphere centered at intersection of equator and central meridian.

 Distance scale is distorted as longitude from central meridian of origin increases, 
 but is less than +0.4% at +/- 5 degrees longititude from central meridian.

 This means if we constructe a Cartesian coordinate system for the robot centered on Baltimore,
 even if the robot was driven all the way to Ohio, distances would still be accurate to within 0.4%.

 For mathematical details, see https://web.archive.org/web/20130930144834/http://www.mercator99.webspace.virginmedia.com/mercator.pdf
 */ 
class CoordinateSystem {
public:
    CoordinateSystem(Coordinates origin);

    Vector2 project(Coordinates coordinates);
    Coordinates project(Vector2 position);

    friend std::ostream& operator<<(std::ostream& output, const CoordinateSystem& coordinate_system);

private:
    constexpr double radians(double degrees) const;
    constexpr double degrees(double radians) const;

    // mean radius from https://en.wikipedia.org/wiki/Earth
    static constexpr double earth_mean_radius_m = 6371000.0;

    Coordinates origin;
};

}  // namespace common

#endif
