#ifndef ULTRASONIC_SENSOR_SIM_IMPL_HPP
#define ULTRASONIC_SENSOR_SIM_IMPL_HPP

#include "distance_sensor_model.hpp"
#include "ultrasonic_sensor.hpp"
#include "vector2.hpp"

namespace sim {

class UltrasonicSensorSimImpl final : public hal::UltrasonicSensor {
public:
    UltrasonicSensorSimImpl(int id, DistanceSensorModel model, double fov, int samples);

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

}  // namespace sim

#endif