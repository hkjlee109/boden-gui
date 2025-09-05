#pragma once

#include "osx_queue.hpp"

#include <thread>

namespace platform {

class osx_backend_t
{
public:
    osx_backend_t(platform::osx_queue_t &queue);
    ~osx_backend_t();
    
    void start();

private:
    std::thread _thread;
    platform::osx_queue_t &_queue;
    
    int main();
};

} // platform
