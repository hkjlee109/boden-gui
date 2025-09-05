#pragma once

#include <boden/backend.hpp>

namespace platform {

class osx_backend_t
{
public:
    osx_backend_t();
    ~osx_backend_t();
    
    bool try_run();

private:
    boden::backend_t _backend;
};

} // platform
