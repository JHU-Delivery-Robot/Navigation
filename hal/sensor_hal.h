#ifndef _SENSOR_HAL_H_
#define _SENSOR_HAL_H_

#include "hal_types.h"

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
