#ifndef _HAL_TYPES_H_
#define _HAL_TYPES_H_

namespace hal {

typedef unsigned long OdomReading;
typedef void (*DRDYCallback)(void);
typedef void (*OdomCallback)(OdomReading);


#define DRDY_CALLBACK_FUNC(name) void name(void)
#define ODOM_CALLBACK(name) void name(OdomReading reading)

}

#endif
