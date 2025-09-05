#include "osx_queue.hpp"

namespace platform {

osx_queue_t::osx_queue_t()
{
}

osx_queue_t::~osx_queue_t()
{
}

void osx_queue_t::push(const platform::osx_event_t &event)
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(event);
    }
    _condition_variable.notify_one();
}

void osx_queue_t::pop()
{
    std::unique_lock<std::mutex> lock(_mutex);
    if(!_queue.empty())
    {
        _queue.pop();
    }
}

void osx_queue_t::wait()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _condition_variable.wait(lock, [&] { return !_queue.empty(); });
}

const platform::osx_event_t osx_queue_t::front()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.front();
}

} // platform
