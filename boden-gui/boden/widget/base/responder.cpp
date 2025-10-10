#include "responder.hpp"

namespace boden {
namespace widget {
namespace base {

responder_t::responder_t()
{
}

responder_t::~responder_t()
{
}

void responder_t::mouse_down(const boden::event_t &ev)
{
}

void responder_t::mouse_dragged(const boden::event_t &ev)
{
}

void responder_t::mouse_entered(const boden::event_t &ev)
{
}

void responder_t::mouse_exited(const boden::event_t &ev)
{
}

void responder_t::mouse_moved(const boden::event_t &ev)
{
}

void responder_t::mouse_up(const boden::event_t &ev)
{
}

void responder_t::system_event(const boden::system_event_t &ev)
{
}

bool responder_t::accepts_first_responder()
{
    return false;
}

bool responder_t::become_first_responder()
{
    return false;
}

bool responder_t::resign_first_responder()
{
    return false;
}

} // base
} // widget
} // boden
