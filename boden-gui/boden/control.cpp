#include "control.hpp"

namespace boden {

control_t::control_t()
    : view_t{},
      _enabled{true}
{
}

control_t::control_t(const boden::layout::rect_t &frame)
    : view_t{frame},
      _enabled{true}
{
}

control_t::~control_t()
{
}

void control_t::send_actions(control_event_t event)
{
    if(!is_enabled()) 
    {
        return;
    }

    for(auto action : _actions[control_event_t::touch_down])
    {
        action();
    }   
}

bool control_t::is_enabled() const
{
    return _enabled;
}

void control_t::set_enabled(bool enabled)
{
    _enabled = enabled;
}

} // boden
