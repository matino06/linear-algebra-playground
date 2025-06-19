#pragma once

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

class Monitor
{
private:
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

public:
    void lock();
    void unlock();
    void wait();
    void notify();
    void reset();
};
