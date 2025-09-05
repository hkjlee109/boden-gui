#include "osx_backend.hpp"

namespace platform {

osx_backend_t::osx_backend_t(platform::osx_queue_t &queue)
    : _queue{queue}
{
}

osx_backend_t::~osx_backend_t()
{
    if(_thread.joinable())
    {
        _thread.join();
    }
}

void osx_backend_t::start()
{
    _thread = std::thread(&osx_backend_t::main, this);
}

int osx_backend_t::main()
{
    std::string msg;
    while(true)
    {
        _queue.wait();
        auto event = _queue.front();
        _queue.pop();

        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

} // platform
