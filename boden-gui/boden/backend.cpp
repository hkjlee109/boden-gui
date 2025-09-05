#include "backend.hpp"

namespace boden {

class backend_t::impl_t
{
public:
    impl_t();
    ~impl_t();

    bool try_run();

private:
};

backend_t::impl_t::impl_t()
{
}

backend_t::impl_t::~impl_t()
{
}

bool backend_t::impl_t::try_run()
{
    return true;
}

backend_t::backend_t()
{
    _impl = std::make_unique<backend_t::impl_t>();
}

backend_t::~backend_t()
{
}

bool backend_t::try_run()
{
   return _impl->try_run();
}

} // boden
