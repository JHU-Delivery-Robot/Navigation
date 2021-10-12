#include "polygon.hpp"

#include <cmath>

namespace sim {

Polygon::Polygon(std::vector<common::Vector2> points) : points(points) {
    for (size_t i = 0; i < points.size(); i++) {
        common::Vector2 start = points[i];
        common::Vector2 end = (i < points.size() - 1) ? points[i + 1] : points[0];  // wrap around to close polygon

        common::Vector2 delta = end - start;
        deltas.push_back(delta);
    }
}

// intersection of ray starting at p going in direction q with specified polygon side
std::tuple<bool, double> Polygon::intersect(common::Vector2 p, common::Vector2 q, size_t side) const {
    common::Vector2 r = points[side];
    common::Vector2 s = deltas[side];

    double v = q.y * s.x - q.x * s.y;
    bool parallel = std::abs(v) < 1e-6;

    if (parallel) {
        return {false, 0.0};
    }

    common::Vector2 position_translation = p - r;

    double beta = (q.y * position_translation.x - q.x * position_translation.y) / v;

    if (beta < 0.0 || beta > 1.0) {
        return {false, 0.0};
    }

    double alpha = (s.y * position_translation.x - s.x * position_translation.y) / v;
    double distance = alpha * q.magnitude();

    return {distance > 0, distance};
}

// closest intersection of ray starting at p going in direction q with polygon
std::tuple<bool, double> Polygon::intersect(common::Vector2 position, common::Vector2 direction) const {
    bool intersection_found = false;
    double shortest_distance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < points.size(); i++) {
        auto [intersected, distance] = intersect(position, direction, i);

        if (intersected && distance < shortest_distance) {
            shortest_distance = distance;
            intersection_found = true;
        }
    }

    return {intersection_found, shortest_distance};
}

std::ostream &operator<<(std::ostream &output, const Polygon &polygon) {
    output << "[";

    for (size_t i = 0; i < polygon.points.size(); i++) {
        output << "[" << polygon.points[i].x << ", " << polygon.points[i].y << "]";
        if (i < polygon.points.size() - 1) {
            output << ", ";
        }
    }

    output << "]";
    return output;
}

}  // namespace sim
