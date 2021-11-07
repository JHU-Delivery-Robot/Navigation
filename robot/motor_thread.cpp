#include "motor_thread.hpp"
#include <thread>

MotorThread::MotorThread() {
    // Initialize Data Here
    std::thread th(start_thread);
    th.join();
}

void MotorThread::start_thread() {
    // Implement Function to run in thread
}