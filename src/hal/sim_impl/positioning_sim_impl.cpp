#include "positioning_sim_impl.hpp"

namespace sim {

PositioningSimImpl::PositioningSimImpl(sim::Simulation* simulation, common::CoordinateSystem coordinate_system)
    : simulation(simulation), coordinate_system(coordinate_system) {}

std::tuple<common::Vector2, double> PositioningSimImpl::getPose() const {
    return simulation->getPose();
}

common::CoordinateSystem PositioningSimImpl::coordinateSystem() const {
    return coordinate_system;
}

}  // namespace sim
