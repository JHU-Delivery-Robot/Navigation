#include "infrared_sensor_sim_impl.hpp"

namespace sim
{

InfraredSensorSimImpl::InfraredSensorSimImpl(int id, DistanceSensorModel model, double fov, int samples)
    : id(id), sensor_model(model), fov(fov), samples(samples), position(0.0, 0.0), heading(0.0) { }

void InfraredSensorSimImpl::updateLocation(common::Vector2 position, double heading) {
    this->position = position;
    this->heading = heading;
}

int InfraredSensorSimImpl::getID() const {
    return id;
}

double InfraredSensorSimImpl::read() {
    double shortest_distance = std::numeric_limits<double>::max();

    for (int i = -samples; i <= samples; i += 1) {
        double angle = heading + i*fov/(2*samples);
        common::Vector2 direction = common::Vector2::polar(angle, 1.0);
        double distance = sensor_model.sample(position, direction);

        if (distance < shortest_distance) {
            shortest_distance = distance;
        }
    }

    return shortest_distance;
}

}
