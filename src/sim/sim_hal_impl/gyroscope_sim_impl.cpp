#include "gyroscope_sim_impl.hpp"

namespace sim
{

GyroscopeSimImpl::GyroscopeSimImpl() : heading(0.0) { }

void GyroscopeSimImpl::setHeading(double heading) {
    this->heading = heading;
}

void GyroscopeSimImpl::updateHeading(double heading_delta) {
    heading += heading_delta;
}

double GyroscopeSimImpl::angle() {
    return heading;
}

}
