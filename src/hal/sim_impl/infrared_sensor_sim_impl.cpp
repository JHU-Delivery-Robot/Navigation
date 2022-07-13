#include "infrared_sensor_sim_impl.hpp"

namespace sim
{

InfraredSensorSimImpl::InfraredSensorSimImpl(int id, sim::Simulation *simulation, DistanceSensorModel model, double fov, int samples)
    : id(id), simulation(simulation), sensor_model(model), fov(fov), samples(samples) { }

int InfraredSensorSimImpl::getID() const {
    return id;
}

double InfraredSensorSimImpl::read() {
    double shortest_distance = std::numeric_limits<double>::max();

    auto [position, angle] = simulation->getPose();

    for (int i = -samples; i <= samples; i += 1) {
        double sample_angle = angle + i*fov/(2*samples);
        common::Vector2 direction = common::Vector2::polar(sample_angle, 1.0);
        double distance = sensor_model.sample(position, direction);

        if (distance < shortest_distance) {
            shortest_distance = distance;
        }
    }

    return shortest_distance;
}

}
