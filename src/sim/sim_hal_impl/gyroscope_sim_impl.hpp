#ifndef GYROSCOPE_SIM_IMPL_HPP
#define GYROSCOPE_SIM_IMPL_HPP

#include <memory>

#include "gyroscope.hpp"

namespace sim
{

class GyroscopeSimImpl final : public hal::Gyroscope
{
public:
    GyroscopeSimImpl();

    void setHeading(double heading);
    void updateHeading(double heading_delta);

    double angle() override;

private:
    double heading;
};

}

#endif
