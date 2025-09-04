#pragma once

namespace boden {

class backend_t
{
public:
    backend_t();
    virtual ~backend_t();

    virtual bool try_run() = 0;

private:
};

} // boden
