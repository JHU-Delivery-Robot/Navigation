#ifndef HAL_PROVIDER_SIM_IMPL_HPP
#define HAL_PROVIDER_SIM_IMPL_HPP

#include <memory>
#include <vector>

#include "common/common.hpp"
#include "hal/hal_provider.hpp"
#include "sim/simulation.hpp"
#include "infrared_sensor_sim_impl.hpp"
#include "lidar_scanner_sim_impl.hpp"
#include "motor_sim_impl.hpp"
#include "positioning_sim_impl.hpp"
#include "ultrasonic_sensor_sim_impl.hpp"

namespace sim {

class HALProviderSimImpl final : public hal::HALProvider {
public:
    class CliffInfraredAssembly final : public hal::HALProvider::CliffInfraredAssembly {
    public:
        CliffInfraredAssembly(sim::Simulation* simulation);

        InfraredSensorSimImpl* front() override;
        InfraredSensorSimImpl* left() override;
        InfraredSensorSimImpl* right() override;
        InfraredSensorSimImpl* back() override;

    private:
        static DistanceSensorModel construct_sensor_model(sim::Simulation* simulation);

        static constexpr double max_range = 1;
        static constexpr double fov = PI / 6;
        static constexpr int samples = 5;

        InfraredSensorSimImpl _front, _left, _right, _back;
    };

    class WheelInfraredAssembly final : public hal::HALProvider::WheelInfraredAssembly {
    public:
        WheelInfraredAssembly(sim::Simulation* simulation);

        InfraredSensorSimImpl* front_left() override;
        InfraredSensorSimImpl* front_right() override;
        InfraredSensorSimImpl* back_left() override;
        InfraredSensorSimImpl* back_right() override;

    private:
        static DistanceSensorModel construct_sensor_model(sim::Simulation* simulation);

        static constexpr double max_range = 1;
        static constexpr double fov = PI / 6;
        static constexpr int samples = 5;

        InfraredSensorSimImpl _front_left, _front_right, _back_left, _back_right;
    };

    class UltrasonicAssembly final : public hal::HALProvider::UltrasonicAssembly {
    public:
        UltrasonicAssembly(sim::Simulation* simulation);

        UltrasonicSensorSimImpl* front() override;
        UltrasonicSensorSimImpl* left() override;
        UltrasonicSensorSimImpl* right() override;
        UltrasonicSensorSimImpl* back() override;

    private:
        static DistanceSensorModel construct_sensor_model(sim::Simulation* simulation);

        static constexpr double max_range = 1;
        static constexpr double fov = PI / 6;
        static constexpr int samples = 5;

        UltrasonicSensorSimImpl _front, _left, _right, _back;
    };

    class MotorAssembly final : public hal::HALProvider::MotorAssembly {
    public:
        MotorAssembly(sim::Simulation *simulation);

        MotorSimImpl* front_left() override;
        MotorSimImpl* front_right() override;
        MotorSimImpl* back_left() override;
        MotorSimImpl* back_right() override;

        void update();
        void reset_odometry() override;

    private:
        MotorSimImpl _front_left, _front_right, _back_left, _back_right;
        sim::Simulation* simulation;
        double last_update_time;
    };

    HALProviderSimImpl(sim::Simulation *simulation, common::CoordinateSystem coordinate_system);

    LidarScannerSimImpl* lidar() override;
    CliffInfraredAssembly* cliff_infrared() override;
    WheelInfraredAssembly* wheel_infrared() override;
    UltrasonicAssembly* ultrasonic() override;
    MotorAssembly* motor_assembly() override;
    PositioningSimImpl* positioning() override;

    void update();

private:
    static constexpr double lidar_max_range = 20.0;
    BeamModel lidar_beam_model;

    LidarScannerSimImpl lidar_impl;
    CliffInfraredAssembly cliff_sensors;
    WheelInfraredAssembly wheel_sensors;
    UltrasonicAssembly ultrasonic_sensors;
    MotorAssembly motors;
    PositioningSimImpl positioning_impl;
};

}  // namespace sim

#endif
