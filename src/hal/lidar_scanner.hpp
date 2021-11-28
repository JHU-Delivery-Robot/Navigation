#ifndef LIDAR_SCANNER_HPP
#define LIDAR_SCANNER_HPP

#include <array>
#include <cstddef>

namespace hal {

class LidarScanner {
public:
    virtual ~LidarScanner() {}

    static constexpr std::size_t SamplesPerRevolution = 720;

    virtual std::array<double, SamplesPerRevolution> read() = 0;
};

}  // namespace hal

#endif
