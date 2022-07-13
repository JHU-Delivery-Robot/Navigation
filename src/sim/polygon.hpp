#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <tuple>
#include <vector>

#include "common/vector2.hpp"
#include "nlohmann/json.hpp"

namespace sim {

class Polygon {
public:
    Polygon(std::vector<common::Vector2> points);

    std::tuple<bool, double> intersect(common::Vector2 position, common::Vector2 direction) const;

    friend std::ostream& operator<<(std::ostream& output, const Polygon& polygon);

    friend nlohmann::adl_serializer<Polygon>;

private:
    std::tuple<bool, double> intersect(common::Vector2 position, common::Vector2 direction, std::size_t side) const;

    std::vector<common::Vector2> points;  // points making up polygon
    std::vector<common::Vector2> deltas;  // vector from each point to the next
};

}  // namespace sim

namespace nlohmann {

template <>
struct adl_serializer<sim::Polygon> {
    static void to_json(nlohmann::ordered_json& json, const sim::Polygon& polygon);
    static sim::Polygon from_json(const nlohmann::ordered_json& json);
};

}  // namespace nlohmann

#endif
