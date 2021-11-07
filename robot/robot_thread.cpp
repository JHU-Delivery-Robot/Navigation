#include <thread>
#include "robot_thread.hpp"

RobotThread::RobotThread() {
    std::thread th(start_thread);
    th.join();
}