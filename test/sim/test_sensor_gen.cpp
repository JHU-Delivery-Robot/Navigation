#include <string>

#include "catch2/catch.hpp"
#include "obstacle_map.hpp"
#include "sensor-gen.hpp"
#include "stb_image_write.h"

using std::string;

#define COORD_TO_IN(c, r, n_c) (r * n_c + c)

#define degreesToRadians(angleDegrees) (angleDegrees * PI / 180.0)
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / PI)
