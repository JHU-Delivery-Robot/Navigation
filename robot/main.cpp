#include <thread>
#include <cstddef>
#include "potential_map.hpp"
#include "types.hpp"
#include "robot_thread.hpp"
#include "motor_thread.hpp"
#include "lidar_thread.hpp"


int main() {
    // TODO: Initialize all data that will be used for the threads
    MotorPositions positions;
    float wheelDiameter;
    int nAngles;
    int qStar;
    int gradientScale;
    Vec2d goal;
    uint16_t *lidarData; // TODO: Implement the data type suggested in spec, refactor as needed
    // TODO: Get these from Kalman filter
    Vec2d position;
    double heading;

    // TODO: Use the actual constructors and initialize all data that will be used for the threads
    LidarThread liThread = LidarThread();
    MotorThread moThread = MotorThread(positions, wheelDiameter, nAngles, qStar, gradientScale,
                                       goal, lidarData, position, heading);

    // TODO: Update lidarData in a loop here? Maybe change lidarData type to specified?
    // TODO: Update position with GPS Data in loop
    // TODO: Update heading with Gyro Data in loop

    liThread.stop_thread();
    moThread.stop_thread();

    return 0;
}
