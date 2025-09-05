#pragma once

#include <memory>

namespace boden {

class backend_t
{
public:
    backend_t();
    virtual ~backend_t();

    virtual bool try_run();

private:
    class impl_t;
    std::unique_ptr<impl_t> _impl;
};

} // boden
