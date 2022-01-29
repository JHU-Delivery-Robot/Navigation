#include "positioning_sim_impl.hpp"

namespace sim {

PositioningSimImpl::PositioningSimImpl() : position(0.0, 0.0), heading(0.0) {}

void PositioningSimImpl::updateLocation(common::Vector2 position, double heading) {
    this->position = position;
    this->heading = heading;
}

std::tuple<common::Vector2, double> PositioningSimImpl::getPose() {
    return {position, heading};
}

void PositioningSimImpl::setPose(common::Vector2 position, double heading) {
    updateLocation(position, heading);
}

}  // namespace sim
