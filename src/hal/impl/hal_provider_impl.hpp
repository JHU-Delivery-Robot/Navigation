#ifndef HAL_PROVIDER_IMPL_HPP
#define HAL_PROVIDER_IMPL_HPP

#include <memory>
#include <vector>
#include <string>

#include "common.hpp"
#include "error_reporting.hpp"
#include "hal_provider.hpp"
#include "TG30_lidar.hpp"

namespace hal {

namespace impl {

class HALProviderImpl final : public HALProvider {
public:
    class LidarConfig {
    public:
        LidarConfig(std::string port, double scan_frequency);
        std::string port;
        double scan_frequency;
    };

    class CliffInfraredAssembly final : public HALProvider::CliffInfraredAssembly {
    public:
        InfraredSensor* front() override;
        InfraredSensor* left() override;
        InfraredSensor* right() override;
        InfraredSensor* back() override;
    };

    class WheelInfraredAssembly final : public hal::HALProvider::WheelInfraredAssembly {
    public:
        InfraredSensor* front_left() override;
        InfraredSensor* front_right() override;
        InfraredSensor* back_left() override;
        InfraredSensor* back_right() override;
    };

    class UltrasonicAssembly final : public hal::HALProvider::UltrasonicAssembly {
    public:
        UltrasonicSensor* front() override;
        UltrasonicSensor* left() override;
        UltrasonicSensor* right() override;
        UltrasonicSensor* back() override;
    };

    class MotorAssembly final : public hal::HALProvider::MotorAssembly {
    public:
        MotorAssembly();

        Motor* front_left() override;
        Motor* front_right() override;
        Motor* back_left() override;
        Motor* back_right() override;

        void reset_odometry() override;
    };

    HALProviderImpl(LidarConfig config, events::ErrorReporting error_reporting);

    void initialize();

    TG30Lidar* lidar() override;
    CliffInfraredAssembly* cliff_infrared() override;
    WheelInfraredAssembly* wheel_infrared() override;
    UltrasonicAssembly* ultrasonic() override;
    MotorAssembly* motor_assembly() override;
    Gyroscope* gyroscope() override;
    Positioning* positioning() override;

private:
    events::ErrorReporting error_reporting;
    LidarConfig config;
    TG30Lidar lidar_impl;
    CliffInfraredAssembly cliff_sensors;
    WheelInfraredAssembly wheel_sensors;
    UltrasonicAssembly ultrasonic_sensors;
    MotorAssembly motors;
};

}  // namespace impl

}  // namespace hal

#endif
