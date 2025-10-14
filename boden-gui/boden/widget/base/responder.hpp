#pragma once

#include <boden/event.hpp>
#include <boden/system_event.hpp>

namespace boden {
namespace widget {
namespace base {

class responder_t
{
public:
    responder_t();
    virtual ~responder_t();

    virtual void mouse_down(const boden::event_t &event);
    virtual void mouse_dragged(const boden::event_t &event);
    virtual void mouse_entered(const boden::event_t &event);
    virtual void mouse_exited(const boden::event_t &event);
    virtual void mouse_moved(const boden::event_t &event);
    virtual void mouse_up(const boden::event_t &event);
    virtual void key_down(const boden::event_t &event);
    virtual void key_up(const boden::event_t &event);

    virtual void system_event(const boden::system_event_t &system_event);
    
    virtual bool accepts_first_responder();
    virtual bool become_first_responder();
    virtual bool resign_first_responder();

private:
};

} // base
} // widget
} // boden
