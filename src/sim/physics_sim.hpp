#ifndef PHYSICS_SIM_HPP
#define PHYSICS_SIM_HPP

#include <tuple>

#include "differential_odometry.hpp"
#include "hal_provider.hpp"
#include "vector2.hpp"

namespace sim {

class PhysicsSim {
public:
    PhysicsSim(hal::HALProvider* hal);
    void update();

    std::tuple<common::Vector2, double> getPose();
    void setPose(common::Vector2, double);

private:
    hal::HALProvider* hal;
    common::DifferentialOdometry odometry;
};

}  // namespace sim

#endif
