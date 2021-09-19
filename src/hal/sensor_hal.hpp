#ifndef _SENSOR_HAL_HPP_
#define _SENSOR_HAL_HPP_

#include "hal_types.hpp"

namespace hal {

class Lidar {
protected:
    void data_ready();
public:
    Lidar();
    void set_callback(DRDYCallback func);
};

extern Lidar lidar;

}

#endif
