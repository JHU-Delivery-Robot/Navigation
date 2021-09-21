#ifndef INFRARED_SENSOR_SIM_IMPL_HPP
#define INFRARED_SENSOR_SIM_IMPL_HPP

#include "infrared_sensor.hpp"
#include "distance_sensor_model.hpp"
#include "vector2.hpp"

namespace sim
{

class InfraredSensorSimImpl : public hal::InfraredSensor
{
public:
    InfraredSensorSimImpl(int id, DistanceSensorModel model, double fov, int samples);

    void updateLocation(common::Vector2 position, double heading);
    int getID() const;

    double read() override;

private:
    int id;

    DistanceSensorModel sensor_model;
    double fov;
    int samples;

    common::Vector2 position;
    double heading;
};

}

#endif
