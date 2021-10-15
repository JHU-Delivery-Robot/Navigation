#ifndef GPS_SIM_IMPL_HPP
#define GPS_SIM_IMPL_HPP

#include <memory>

#include "gps.hpp"

namespace sim {

class GPSSimImpl final : public hal::GPS {
public:
    GPSSimImpl();

    void updateLocation(common::Vector2 position, double heading);

    std::tuple<common::Vector2, double> location() override;

private:
    common::Vector2 position;
    double heading;
};

}  // namespace sim

#endif
