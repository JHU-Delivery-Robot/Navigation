#include <thread>
#include "potential_map.hpp"
#include "types.hpp"
#include "motor_thread.hpp"
#include "lidar_thread.hpp"

// no literal map (2D array representing grid), but we're constantly fed
// robot's location and goal points
// (either receive all goal points at once, or one at a time as the robot moves)

int main() {
  LidarThread liThread = LidarThread();
  MotorThread moThread = MotorThread();
  return 0;
}
