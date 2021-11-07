#include "lidar_thread.hpp"
#include <thread>

LidarThread::LidarThread() {
    // Initialize Data Here
    std::thread th(start_thread);
    th.join();
}

void LidarThread::start_thread() {
    // Implement Function to run in thread
}