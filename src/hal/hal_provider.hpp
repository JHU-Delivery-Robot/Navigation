#ifndef HAL_PROVIDER_HPP
#define HAL_PROVIDER_HPP

#include <memory>

#include "positioning.hpp"
#include "infrared_sensor.hpp"
#include "lidar_scanner.hpp"
#include "motor.hpp"
#include "ultrasonic_sensor.hpp"

namespace hal {

class HALProvider {
public:
    class CliffInfraredAssembly {
    public:
        virtual ~CliffInfraredAssembly(){};

        virtual InfraredSensor* front() = 0;
        virtual InfraredSensor* left() = 0;
        virtual InfraredSensor* right() = 0;
        virtual InfraredSensor* back() = 0;
    };

    class WheelInfraredAssembly {
    public:
        virtual ~WheelInfraredAssembly(){};

        virtual InfraredSensor* front_left() = 0;
        virtual InfraredSensor* front_right() = 0;
        virtual InfraredSensor* back_left() = 0;
        virtual InfraredSensor* back_right() = 0;
    };

    class UltrasonicAssembly {
    public:
        virtual ~UltrasonicAssembly(){};

        virtual UltrasonicSensor* front() = 0;
        virtual UltrasonicSensor* left() = 0;
        virtual UltrasonicSensor* right() = 0;
        virtual UltrasonicSensor* back() = 0;
    };

    class MotorAssembly {
    public:
        virtual ~MotorAssembly(){};

        virtual Motor* front_left() = 0;
        virtual Motor* front_right() = 0;
        virtual Motor* back_left() = 0;
        virtual Motor* back_right() = 0;

        virtual void reset_odometry() = 0;
    };

    virtual ~HALProvider() {}

    virtual LidarScanner* lidar() = 0;
    virtual CliffInfraredAssembly* cliff_infrared() = 0;
    virtual WheelInfraredAssembly* wheel_infrared() = 0;
    virtual UltrasonicAssembly* ultrasonic() = 0;
    virtual MotorAssembly* motor_assembly() = 0;
    virtual Positioning* positioning() = 0;
};

}  // namespace hal

#endif
