#ifndef POSITIONING_SIM_IMPL_HPP
#define POSITIONING_SIM_IMPL_HPP

#include <memory>

#include "positioning.hpp"

namespace sim {

class PositioningSimImpl final : public hal::Positioning {
public:
    PositioningSimImpl();

    void updateLocation(common::Vector2 position, double heading);

    std::tuple<common::Vector2, double> getPose() override;
    void setPose(common::Vector2 position, double heading) override;

private:
    common::Vector2 position;
    double heading;
};

}  // namespace sim

#endif
