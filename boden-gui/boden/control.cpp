#include "control.hpp"

namespace boden {

control_t::control_t()
    : view_t{}
{
}

control_t::control_t(const boden::layout::rect_t &frame)
    : view_t{frame}
{
}

control_t::~control_t()
{
}

const std::unordered_map<control_event_t, std::function<void()>> & control_t::get_actions() const 
{
    return _actions;
}

} // boden
