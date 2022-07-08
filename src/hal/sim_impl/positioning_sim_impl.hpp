#ifndef POSITIONING_SIM_IMPL_HPP
#define POSITIONING_SIM_IMPL_HPP

#include <memory>

#include "hal/positioning.hpp"
#include "sim/simulation.hpp"

namespace sim {

class PositioningSimImpl final : public hal::Positioning {
public:
    PositioningSimImpl(sim::Simulation *simulation, common::CoordinateSystem coordinate_system);

    std::tuple<common::Vector2, double> getPose() const override;
    common::CoordinateSystem coordinateSystem() const override;

private:
    sim::Simulation *simulation;
    common::CoordinateSystem coordinate_system;
};

}  // namespace sim

#endif
