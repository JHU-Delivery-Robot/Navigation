#ifndef LIDAR_SCANNER_HPP
#define LIDAR_SCANNER_HPP

#include <array>

namespace hal
{

class LidarScanner
{
public:
    virtual ~LidarScanner();

    static constexpr size_t SamplesPerRevolution = 720;

    virtual std::array<double, SamplesPerRevolution> read() = 0;
};

}

#endif
