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
    : _image_manager{},
      _font_manager{},
      _needs_display{false}
{
    _impl = std::make_unique<backend_t::impl_t>();
}

backend_t::~backend_t()
{
}

bool backend_t::needs_display() const
{
    return _needs_display;
}

void backend_t::set_needs_display(bool needs)
{
    _needs_display = needs;
}

bool backend_t::needs_system() const
{
    return !_system_event_queue.empty();
}

void backend_t::enqueue_system_event(const boden::system_event_t &system_event)
{
    _system_event_queue.push(system_event);
}
    
std::optional<boden::system_event_t> backend_t::dequeue_system_event()
{
    if(_system_event_queue.empty())
    {
        return std::nullopt;
    }

    system_event_t event = _system_event_queue.front();
    _system_event_queue.pop();
    return event;
}

} // boden
