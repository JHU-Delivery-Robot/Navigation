#include "gps_sim_impl.hpp"

namespace sim {

GPSSimImpl::GPSSimImpl() : position(0.0, 0.0), heading(0.0) {}

void GPSSimImpl::updateLocation(common::Vector2 position, double heading) {
    this->position = position;
    this->heading = heading;
}

std::tuple<common::Vector2, double> GPSSimImpl::location() {
    return {position, heading};
}

}  // namespace sim
