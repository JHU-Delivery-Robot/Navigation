#ifndef ROBOT_THREAD_HPP
#define ROBOT_THREAD_HPP

#include <thread>

class RobotThread {
private:
    std::thread th;
public:
    RobotThread();
    virtual void start_thread() = 0;
    void stop_thread();
};

#endif
