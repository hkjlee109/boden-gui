#include "utils.hpp"

#include <boden/layout/point.hpp>
#include <AppKit/AppKit.h>

namespace platform {

bool utils_t::convert_to_event(void *in, boden::event_t *out)
{
    if(in == nullptr || out == nullptr)
    {
        return false;
    }
    
    NSEvent* event = (__bridge NSEvent*)(in);
    out->location = boden::layout::point_t(event.locationInWindow.x, event.locationInWindow.y);

    return true;
}

} // platform
