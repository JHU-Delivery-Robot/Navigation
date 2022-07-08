#ifndef ULTRASONIC_SENSOR_SIM_IMPL_HPP
#define ULTRASONIC_SENSOR_SIM_IMPL_HPP

#include "distance_sensor_model.hpp"
#include "hal/ultrasonic_sensor.hpp"
#include "common/vector2.hpp"
#include "sim/simulation.hpp"

namespace sim {

class UltrasonicSensorSimImpl final : public hal::UltrasonicSensor {
public:
    UltrasonicSensorSimImpl(int id, sim::Simulation *simulation, DistanceSensorModel model, double fov, int samples);

    int getID() const;

    double read() override;

private:
    int id;

    sim::Simulation *simulation;
    DistanceSensorModel sensor_model;

    double fov;
    int samples;
};

}  // namespace sim

#endif
