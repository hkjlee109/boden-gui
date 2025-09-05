#pragma once

#include "osx_event.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>

namespace platform {

class osx_queue_t
{
public:
    osx_queue_t();
    ~osx_queue_t();

    void push(const platform::osx_event_t &event);
    void pop();
    
    void wait();
    const platform::osx_event_t front();

private:
    std::mutex _mutex;
    std::queue<platform::osx_event_t> _queue;
    std::condition_variable _condition_variable;
};

} // platform
