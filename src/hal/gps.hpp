#ifndef GPS_HPP
#define GPS_HPP

#include <tuple>

#include "vector2.hpp"

namespace hal {

class GPS {
public:
    virtual ~GPS() {}

    virtual std::tuple<common::Vector2, double> location() = 0;
};

}  // namespace hal

#endif
