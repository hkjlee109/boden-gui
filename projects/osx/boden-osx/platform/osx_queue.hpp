#pragma once

#include <boden/event.hpp>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace platform {

class osx_queue_t
{
public:
    osx_queue_t();
    ~osx_queue_t();

    void push(const boden::event_t &event);
    void pop();
    
    void wait();
    const boden::event_t front();

private:
    std::mutex _mutex;
    std::queue<boden::event_t> _queue;
    std::condition_variable _condition_variable;
};

} // platform
