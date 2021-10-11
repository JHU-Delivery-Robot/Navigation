#ifndef GYROSCOPE_HPP
#define GYROSCOPE_HPP

namespace hal
{

class Gyroscope
{
public:
    virtual ~Gyroscope() { }
    
    virtual double angle() = 0;
};

}

#endif
