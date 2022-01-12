#include <thread>
#include "types.hpp"
#include "motor_loop.hpp"
#include "lidar_loop.hpp"

int main() {
    // TODO: Initialize all data that will be used for the threads
    MotorPositions positions;
    float wheelDiameter = 0.0;
    int nAngles = 1;
    int qStar = 1;
    int gradientScale = 1;
    Vec2d goal;

    // TODO: Implement the data type suggested in spec, refactor as needed
    uint16_t *lidarData;

    // TODO: Get the following two from Kalman filter
    Vec2d position;
    double heading;

    LidarLoop lidarLoop = LidarLoop(20);
    MotorLoop motorLoop = MotorLoop(positions, wheelDiameter, nAngles, qStar, gradientScale,
                                    goal, lidarData, position, heading, 20);

    std::thread lidarThread(&LidarLoop::loop, &lidarLoop);
    std::thread motorThread(&MotorLoop::loop, &motorLoop);

    // TODO: while loop to get lidar data, position, heading, and goal goes here

    lidarLoop.cancel();
    motorLoop.cancel();

    lidarThread.join();
    motorThread.join();

    return 0;
}
