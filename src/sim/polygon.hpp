#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include <tuple>

#include "../common/vector2.hpp"

namespace sim {

class Polygon {
public:
    Polygon(std::vector<common::Vector2> points);

    std::tuple<bool, double> intersect(common::Vector2 position, common::Vector2 direction) const;

private:
    std::tuple<bool, double> intersect(common::Vector2 position, common::Vector2 direction, size_t side) const;

    std::vector<common::Vector2> points; // points making up polygon
    std::vector<common::Vector2> deltas; // vector from each point to the next
};

}

#endif
