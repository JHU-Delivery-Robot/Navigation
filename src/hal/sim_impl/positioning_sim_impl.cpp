#include "positioning_sim_impl.hpp"

namespace sim {

PositioningSimImpl::PositioningSimImpl(sim::Simulation* simulation) : simulation(simulation) {}

std::tuple<common::Vector2, double> PositioningSimImpl::getPose() {
    return simulation->getPose();
}

}  // namespace sim
