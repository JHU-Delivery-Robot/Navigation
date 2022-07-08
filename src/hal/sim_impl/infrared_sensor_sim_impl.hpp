#ifndef INFRARED_SENSOR_SIM_IMPL_HPP
#define INFRARED_SENSOR_SIM_IMPL_HPP

#include <memory>

#include "hal/infrared_sensor.hpp"
#include "distance_sensor_model.hpp"
#include "common/vector2.hpp"
#include "sim/simulation.hpp"

namespace sim
{

class InfraredSensorSimImpl final : public hal::InfraredSensor
{
public:
    InfraredSensorSimImpl(int id, sim::Simulation *simulation, DistanceSensorModel model, double fov, int samples);

    int getID() const;

    double read() override;

private:
    int id;

    sim::Simulation *simulation;
    DistanceSensorModel sensor_model;
    double fov;
    int samples;
};

}

#endif
