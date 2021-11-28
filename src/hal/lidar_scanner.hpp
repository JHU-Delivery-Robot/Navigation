#ifndef LIDAR_SCANNER_HPP
#define LIDAR_SCANNER_HPP

#include <cstddef>
#include <memory>
#include <vector>

namespace hal {

class LidarScanner {
public:
    /**
     * Sample point from a scan, containing:
     *     - distance in meters
     *     - angle in radians
     */
    class SamplePoint {
    public:
        double distance;
        double angle;

        SamplePoint(uint16_t distance, double angle) : distance(distance), angle(angle) {}
    };

    using Scan = std::shared_ptr<std::vector<SamplePoint>>;

    virtual ~LidarScanner() {}

    virtual Scan read() = 0;
};

}  // namespace hal

#endif
