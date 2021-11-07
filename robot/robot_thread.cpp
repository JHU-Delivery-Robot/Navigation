#include "robot_thread.hpp"

RobotThread::RobotThread() {

}

void RobotThread::stop_thread() {
    th.join();
}
