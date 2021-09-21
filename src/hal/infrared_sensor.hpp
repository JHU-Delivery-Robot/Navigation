#ifndef INFRARED_SENSOR_HPP
#define INFRARED_SENSOR_HPP

namespace hal
{

class InfraredSensor
{
public:
    virtual ~InfraredSensor();
    
    virtual double read() = 0;
};

}

#endif
