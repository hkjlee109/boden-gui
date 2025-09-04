#pragma once

#include <boden/backend.hpp>

namespace platform {

class osx_backend_t : public boden::backend_t
{
public:
    osx_backend_t();
    ~osx_backend_t() override;
    
    bool try_run() override;

private:
};

} // platform
