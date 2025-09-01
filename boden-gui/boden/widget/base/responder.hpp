#pragma once

#include <boden/event.hpp>

namespace boden {
namespace widget {
namespace base {

class responder_t
{
public:
    responder_t();
    virtual ~responder_t();

    virtual void mouse_down(const boden::event_t &ev);
    virtual void mouse_up(const boden::event_t &ev);
    
private:
};

} // base
} // widget
} // boden
