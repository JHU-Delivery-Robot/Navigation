#ifndef ULTRASONIC_SENSOR_HPP
#define ULTRASONIC_SENSOR_HPP

namespace hal
{

class UltrasonicSensor
{
public:
    virtual ~UltrasonicSensor() { }
    
    virtual double read() = 0;
};

}

#endif
