#include "Monitor.h"

void Monitor::lock()
{
    mtx.lock();
}

void Monitor::unlock()
{
    mtx.unlock();
}

void Monitor::wait()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]
            { return ready; });
}

void Monitor::notify()
{
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
    cv.notify_all();
}

void Monitor::reset()
{
    std::lock_guard<std::mutex> lock(mtx);
    ready = false;
}