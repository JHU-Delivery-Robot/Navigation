#ifndef PHYSICS_SIM_HPP
#define PHYSICS_SIM_HPP

#include <tuple>

#include "differential_odometry.hpp"
#include "hal_provider_sim_impl.hpp"
#include "vector2.hpp"

namespace sim {

class PhysicsSim {
public:
    PhysicsSim(HALProviderSimImpl* sim_hal);
    void update(double time_delta);

    std::tuple<common::Vector2, double> getPose();
    void setPose(common::Vector2, double);

private:
    HALProviderSimImpl* sim_hal;
    common::DifferentialOdometry odometry;
};

}  // namespace sim

#endif
